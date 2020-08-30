#include "VarAnalyzer.h"

using namespace slib;
using namespace slib::sbio;

srange arrange(salign *a, size_t &len) {
    return srange((a->ref.dir?len-(a->aligned.end+1):a->aligned.begin),
                  (a->ref.dir?len-a->aligned.begin-1:a->aligned.end));
}

inline size_t dif_range(sregion &reg, srange &rng) {
    sforeach(reg) {
        if (rng.end < it->begin) break;
        if (it->include(rng)) return 0;
        else if(it->overlap(*it)) {
            if (it->include(rng.begin)) rng.begin = it->end+1;
            else rng.end = it->begin-1;
        }
    }
    return rng.length();
}

align_map::align_map() {}
align_map::align_map(size_t l) : length(l) {}
align_map::align_map(const align_map &am) {
    length = am.length;
    aligns = am.aligns;
}
align_map::~align_map() {}

align_map &align_map::operator=(const align_map &am) {
    length = am.length;
    aligns = am.aligns;
    return *this;
}

size_t align_map::append_len(salign *a) {
    srange range = arrange(a, length);
    if (aligns.empty() ||
        range.end < aligns.first().first.begin ||
        aligns.last().first.end < range.begin)
        return a->aligned.length(true);
    else {
        sforeach(aligns) {
            if (range.end < it->first.begin) break;
            if (it->first.include(range)) return 0;
            else if(it->first.overlap(range)) {
                if (it->first.include(range.begin)) range.begin = it->first.end+1;
                else range.end = it->first.begin-1;
            }
        }
    }
    return range.length(true);
}

void align_map::append(salign *a) {
	
    srange range = arrange(a, length);
    if (aligns.empty() || aligns.last().first.end < range.begin)
        aligns.add(std::make_pair(range, a));
    else if(range.end < aligns.first().first.begin)
        aligns.insert(0, std::make_pair(range, a));
    else {
        sforeach(aligns) {
            if(it->first.overlap(range)) {
                if (it->first.include(range.begin))
                    aligns.insert(it-aligns.begin()+1, std::make_pair(range, a));
                else
                    aligns.insert(it-aligns.begin(), std::make_pair(range, a));
                break;
            }
            else if (it < aligns.end()-1 && it->first.end < range.begin && range.end < (it+1)->first.begin) {
                aligns.insert(it-aligns.begin()+1, std::make_pair(range, a));
                break;
            }
        }
    }
}

bool align_map::operator<(const align_map &am) const {
    return aligns[0].first < am.aligns[0].first;
}
bool align_map::operator==(const align_map &am) const {
    return length == am.length && aligns == am.aligns;
}

va_pair::va_pair() { idx1 = -1; idx2 = -1; }
va_pair::va_pair(int i, svar_data &v) {
    idx1 = i; idx2 = -1; var = v;
}
va_pair::va_pair(int i1, int i2, svar_data &v) {
    idx1 = i1; idx2 = i2; var = v;
}
va_pair::~va_pair() {}


bool va_pair::operator<(const va_pair &vap) const {
    if (idx1 != idx2) return idx1 < vap.idx1;
    return idx2 < vap.idx2;
}
bool va_pair::operator==(const va_pair &vap) const {
    return idx1 == vap.idx1 && idx2 == vap.idx2;
}

inline void trimHead(salign *al, int len, va_param *p) {
    while (-1 < len) {
        auto &cig = al->cigars.first();
        if (len < cig.length) break;
        else {
            len -= cig.length;
            al->aligned.begin += cig.length;
            al->ref.begin += cig.length;
            al->score -= (cig.option==scigar::PMATCH?p->search_par.aln_par.pm_score:p->search_par.aln_par.am_score)*cig.length;
            al->cigars.setOffset(1);
            do {
                cig = al->cigars.first();
                if (cig.option==scigar::MMATCH) {
                    al->aligned.begin += cig.length;
                    al->ref.begin += cig.length;
                    al->score -= p->search_par.aln_par.mm_score*cig.length;
                }
                else if (cig.option==scigar::DELETION || cig.option==scigar::INSERTION) {
                    if (cig.option==scigar::DELETION) al->ref.begin += cig.length;
                    else al->aligned.begin += cig.length;
                    al->score -= p->search_par.aln_par.gap_score+p->search_par.aln_par.gap2_score*(cig.length-1);
                }
                else break;
                al->cigars.setOffset(1);
            }
            while (cig.option!=scigar::MATCH && cig.option!=scigar::PMATCH);
        }
    }
}

inline void trimTail(salign *al, int len, va_param *p) {
    while (-1 < len) {
        auto &cig = al->cigars.last();
        if (len < cig.length) break;
        else {
            len -= cig.length;
            al->ref.end -= cig.length;
            al->aligned.end -= cig.length;
            al->score -= (cig.option==scigar::PMATCH?p->search_par.aln_par.pm_score:p->search_par.aln_par.am_score)*cig.length;
            al->cigars.resize(al->cigars.size()-1);
            do {
                cig = al->cigars.last();
                if (cig.option==scigar::MMATCH) {
                    al->ref.end -= cig.length;
                    al->aligned.end -= cig.length;
                    al->score -= p->search_par.aln_par.mm_score*cig.length;
                }
                else if (cig.option==scigar::DELETION || cig.option==scigar::INSERTION) {
                    if (cig.option==scigar::DELETION) al->ref.end -= cig.length;
                    else al->aligned.end -= cig.length;
                    al->score -= p->search_par.aln_par.gap_score+p->search_par.aln_par.gap2_score*(cig.length-1);
                }
                else break;
                al->cigars.resize(al->cigars.size()-1);
            }
            while (cig.option!=scigar::MATCH && cig.option!=scigar::PMATCH);
        }
    }
}

inline void map2res(align_map &am, va_result &vr, SBExtend *le, va_param *p) {
    vr.total_score = 0;
    vr.length = am.length;
    srange range = am.aligns[0].first;
    salign al = *am.aligns[0].second;
    if (1 < am.aligns.size()) {
        sforin(i, 1, am.aligns.size()) {
            auto range_ = am.aligns[i].first;
            auto al_ = *am.aligns[i].second;
            if(range_.begin <= range.end) {
                auto len = range.end-range_.begin+1;
                if (al.ref.dir) trimHead(&al, len, p);
                else trimTail(&al, len, p);
                if (al_.ref.dir) trimTail(&al_, len, p);
                else trimHead(&al_, len, p);
                range = arrange(&al, am.length);
                range_ = arrange(&al_, am.length);
                if (range_.begin <= range.end) {
                    len = range.end-range_.begin+1;
                    bool post = true;
                    if (al.ref.idx != al_.ref.idx && al_.ref.idx < al.ref.idx) post = false;
                    else if(al.ref.idx == al_.ref.idx && al_.ref.begin < al.ref.begin) post = false;
                    if (post) {
                        if (al_.ref.dir) {
                            auto &cig = al_.cigars.last();
                            al_.ref.end -= len;
                            al_.aligned.end -= len;
                            al_.score -= (cig.option==scigar::PMATCH?p->search_par.aln_par.pm_score:p->search_par.aln_par.am_score)*len;
                            cig.length -= len;
                        }
                        else {
                            auto &cig = al_.cigars.first();
                            al_.ref.begin += len;
                            al_.aligned.begin += len;
                            al_.score -= (cig.option==scigar::PMATCH?p->search_par.aln_par.pm_score:p->search_par.aln_par.am_score)*len;
                            cig.length -= len;
                        }
                    }
                    else {
                        if (!al.ref.dir) {
                            auto &cig = al.cigars.last();
                            al.ref.end-= len;
                            al.aligned.end -= len;
                            al.score -= cig.option==(scigar::PMATCH?p->search_par.aln_par.pm_score:p->search_par.aln_par.am_score)*len;
                            cig.length -= len;
                        }
                        else {
                            auto &cig = al.cigars.first();
                            al.ref.begin += len;
                            al.aligned.begin += len;
                            al.score -= cig.option==(scigar::PMATCH?p->search_par.aln_par.pm_score:p->search_par.aln_par.am_score)*len;
                            cig.length -= len;
                        }
                    }
                }
            }
            vr.aligns.add(al);
            vr.total_score += al.score;
            al = al_;
            range = arrange(&al, vr.length);
            if (i == am.aligns.size()-1) {
                vr.total_score += al.score;
                vr.aligns.add(al);
            }
        }
    }
    else {
        vr.aligns.add(al);
        vr.total_score += al.score;
    }
}

va_result::va_result() {
    total_score = 0;
}
va_result::va_result(const va_result &result) {
    length = result.length;
    aligns = result.aligns;
    total_score = result.total_score;
    svariants = result.svariants;
    lvariants = result.lvariants;
}
va_result::~va_result() {}

va_result &va_result::operator=(const va_result &result) {
    length = result.length;
    aligns = result.aligns;
    total_score = result.total_score;
    svariants = result.svariants;
    lvariants = result.lvariants;
    return *this;
}

bool va_result::operator<(const va_result &res) const { return res.total_score < total_score; }
bool va_result::operator==(const va_result &res) const { return aligns == res.aligns; }

VarAnalyzer::VarAnalyzer(va_param& par) { 
	_par = &par; 
	_que.setParam(&_par->search_par);
	_sbs.setParam(&_par->search_par);
	_sbe.setParam(&_par->search_par);
}
VarAnalyzer::~VarAnalyzer() {}

inline void search(va_param *par, SBQuery &que, SBSearch &sbs, SBExtend &sbe,
                   Array<salign *> &aligns) {
    que.makeStrictTrie();
    sbs.search(&par->ref, &que);
    sbs.makeAlign();
    sforeachi(sbs.aligns) {
        if (sbs.aligns[i].empty()) continue;
        sbe.assemble(par->ref[i/que.count()], &que.query(i%que.count()), &sbs.aligns[i]);
        sforeach(sbs.aligns[i]) {
            if (E_.ref.idx < 0) continue;
            aligns.add(&E_);
        }
    }
    aligns.sort([](const salign *a1, const salign *a2) { return a2->score < a1->score; } );
}

inline void search_map(align_map &map, sint idx,
                       Array<salign *> &aligns, Array<align_map> &maps, va_param *par) {
    integerarray max_idx;
    sint max_len = 0;
    size_t tmp;
    sforin(i, idx+1, aligns.size()) {
        if (aligns[i]->ref.length()+1 < max_len) break;
        if (par->search_par.min_match <= (tmp = map.append_len(aligns[i]))) {
            if (max_len < tmp) {
                max_idx.clear();
                max_idx.add(i);
                max_len = tmp;
            }
            else if(max_len == tmp) max_idx.add(i);
        }
    }
    if (max_idx.empty()) maps.add(map);
    else if (max_idx.size() == 1) {
        map.append(aligns[max_idx[0]]);
        search_map(map, max_idx[0], aligns, maps, par);
    }
    else {
        sforeach(max_idx) {
            align_map map_ = map;
            map_.append(aligns[E_]);
            search_map(map_, E_, aligns, maps, par);
        }
    }
}

int VarAnalyzer::analyze(sobj obj) {
    SBioSeq que_seq;
	if (obj.isNull()) return NO_SAMPLE_ERROR;
	else que_seq.load(DNA_SEQ1, obj);
	_seqlen = que_seq.length();	
	if (_seqlen < _par->search_par.seed_len) return SHORT_SEQUENCE_ERROR;
	_que.setSize(2);
	_que.query(0).swap(que_seq);
	sseq::dcpycompi(_que.query(0), _que.query(1));
	_que.setTotalLength(_seqlen * 2);
	search(_par, _que, _sbs, _sbe, _aligns);
    if(_aligns.empty()) {
        int count = 0;
        sforeach(que_seq) {
            if (E_ == 0 || E_ == 15) ++count;
        }
        if (_seqlen/5 < count) return TOO_MUCH_N_ERROR;
        else return NOT_ALIGNED_ERROR;
    }
    auto max_score = _aligns[0]->score;
    sforeach(_aligns) {
		if(E_->ref.idx < 0) continue;
        if(E_->score < max_score) break;
        align_map map(_seqlen);
        map.append(E_);
        search_map(map, it-_aligns.begin(), _aligns, _maps, _par);
    }
    results.resize(_maps.size());
    sforeachi_(m, _maps) map2res(_maps[m], results[m], &_sbe, _par);
    std::sort(results.begin(), results.end(),
              [](const va_result &vr1, const va_result &vr2) {
                  return vr2.total_score < vr1.total_score;
              });
    bool hasvar = false;
    auto rit = results.begin();
    while (rit < results.end()) {
        if (rit->total_score < results.begin()->total_score) break;
        else if(rit != results.begin() &&
                rit->total_score == results.begin()->total_score &&
                rit->aligns.first() == results.begin()->aligns.first() &&
                rit->aligns.last() == results.begin()->aligns.last()) { ++rit; continue; }
        findVar(*rit, &_que, _par);
        filterVar(*rit, _par);
        if (rit->svariants.size() || rit->lvariants.size()) hasvar = true;
        ++rit;
    }
    results.resize(rit-results.begin());
    if (1 < results.size()) return MULTI_HIT_ERROR;
    else if (hasvar) return ANALYSIS_COMPLETED;
    return NO_VARIANT_ERROR;
}
void VarAnalyzer::reset() {
    _sbs.reset();
    _que.reset();
    _aligns.clear();
    _maps.clear();
    results.clear();
}
