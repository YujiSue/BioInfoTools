#include "VarAnalyzer.h"

inline void findSmallVar(int i, salign &al, ubytearray *que, Array<va_pair> &variants, va_param *par) {
    int rpos = al.ref.begin, qpos = al.aligned.begin;
    sforeach(al.cigars) {
        if (E_.option == scigar::MMATCH) {
            if (par->min_size <= E_.length) {
                svar_data var;
                var.type = E_.length==1?sbio::SNV:sbio::MNV;
                var.pos[0].idx = al.ref.idx;
				var.pos[0].begin = rpos + 1;
				var.pos[0].end = rpos + E_.length;
                var.alt.resize(E_.length, '\0');
                sseq::ddecode1(que->ptr(), qpos, E_.length, (subyte *)&var.alt[0]);
                if (par->isTargeted(&var)) variants.add(i, var);
            }
            rpos += E_.length; qpos += E_.length;
        }
        else if (E_.option == scigar::DELETION) {
            if (par->min_size <= E_.length) {
                svar_data var;
                var.type = sbio::DELETION;
                var.pos[0].idx = al.ref.idx;
				var.pos[0].begin = rpos + 1;
				var.pos[0].end = rpos + E_.length;
                if (par->isTargeted(&var)) variants.add(i, var);
            }
            rpos += E_.length;
        }
        else if (E_.option == scigar::INSERTION) {
            if (par->min_size <= E_.length) {
                svar_data var;
                var.type = sbio::INSERTION;
                var.pos[0].idx = al.ref.idx;
				var.pos[0].begin = rpos + 1;
				var.pos[0].end = rpos + E_.length;
                var.alt.resize(E_.length, '\0');
                sseq::ddecode1(que->ptr(), qpos, E_.length, (subyte *)&var.alt[0]);
                if (par->isTargeted(&var)) variants.add(i, var);
            }
            qpos += E_.length;
        }
        else { rpos += E_.length; qpos += E_.length; }
    }
}

inline void findLargeVariant(int i1, int i2, salign &al1, salign &al2, size_t &length,
                             ubytearray *seq, Array<va_pair> &variants, va_param *par) {
    svar_data var;
    srange range1 = arrange(&al1, length), range2 = arrange(&al2, length);
    if (range1.end+1 < range2.begin) {
        var.alt.resize(range2.begin-range1.end-1, '\0');
        sseq::ddecode1(seq->ptr(), range1.end+1, range2.begin-range1.end-1, (subyte *)&var.alt[0]);
    }
    if (al1.ref.idx == al2.ref.idx) {
        if (al1.ref.dir == al2.ref.dir) {
            if (al1.ref.dir) {
                if (var.alt.length()) sbio::sseq::dcomp(var.alt);
                if (al1.ref.begin < al2.ref.end+1) {
                    var.type |= sbio::DUPLICATION;
                    var.pos[0] = al1.ref;
					var.pos[0].begin++;
					var.pos[0].end = al2.ref.end + 1;
                }
                else if(al2.ref.end+1 < al1.ref.begin) {
                    var.type |= sbio::DELETION;
                    var.pos[0] = al2.ref;
					var.pos[0].begin = al2.ref.end + 2;
					var.pos[0].end = al1.ref.begin;
                }
            }
            else {
                if (al2.ref.begin < al1.ref.begin) {
                    var.type |= sbio::DUPLICATION;
                    var.pos[0] = al2.ref;
					var.pos[0].begin++;
					var.pos[0].end = al1.ref.end + 1;
                }
                else if(al1.ref.end+1 < al2.ref.begin) {
                    var.type |= sbio::DELETION;
                    var.pos[0] = al1.ref;
					var.pos[0].begin = al1.ref.end + 2;
					var.pos[0].end = al2.ref.begin;
                }
            }
        }
        else {
            var.type |= sbio::INVERSION;
            if (al1.ref.dir) {
                var.pos[0] = al1.ref;
				var.pos[0].begin++;
                var.pos[1] = al2.ref;
				var.pos[1].begin++;
            }
            else {
                var.pos[0] = al1.ref;
				var.pos[0].begin = al1.ref.end + 1;
                var.pos[1] = al2.ref;
				var.pos[1].begin = al2.ref.end + 1;
            }
        }
    }
    else {
        var.type |= sbio::TRANSLOCATION;
        if (al1.ref.dir == al2.ref.dir) {
            if (al1.ref.dir) {
                if (var.alt.length()) sbio::sseq::dcomp(var.alt);
                var.pos[0] = al1.ref;
				var.pos[0].begin++;
                var.pos[1] = al2.ref;
				var.pos[1].begin = al2.ref.end + 1;
            }
            else {
                var.pos[0] = al1.ref;
				var.pos[0].begin = al1.ref.end + 1;
                var.pos[1] = al2.ref;
				var.pos[1].begin++;
            }
        }
        else {
            var.type |= sbio::INVERSION;
            if (al1.ref.dir) {
                var.pos[0] = al1.ref;
				var.pos[0].begin++;
                var.pos[1] = al2.ref;
				var.pos[1].begin++;
            }
            else {
                var.pos[0] = al1.ref;
				var.pos[0].begin = al1.ref.end + 1;
                var.pos[1] = al2.ref;
				var.pos[1].begin = al2.ref.end + 1;
            }
        }
    }
    variants.add(i1, i2, var);
}

void findComplexVariant(Array<va_pair> &variants) {
    sforeach(variants) {
        if (it == variants.end()-1 || !E_.var.type) continue;
        for (auto vit_ = it+1; vit_ < variants.end(); ++vit_) {
            if (!vit_->var.type) continue;
            if (((it->var.type&sbio::DELETION && vit_->var.type&sbio::DUPLICATION) ||
                (it->var.type&sbio::DUPLICATION && vit_->var.type&sbio::DELETION)) &&
                it->var.pos[0].idx == vit_->var.pos[0].idx &&
                it->var.pos[0].dir == vit_->var.pos[0].dir) {
                if (it->var.pos[0].dir) {
                    if (vit_->var.pos[0].begin <= it->var.pos[0].begin &&
                        vit_->var.pos[0].begin+vit_->var.pos[0].end <= it->var.pos[0].begin+it->var.pos[0].end &&
                        it->var.pos[0].begin < vit_->var.pos[0].begin+vit_->var.pos[0].end) {
                        svar_data cvar;
                        int dlen, ilen;
                        if (it->var.type&sbio::DELETION) {
                            dlen = (it->var.pos[0].begin+it->var.pos[0].end) - (vit_->var.pos[0].begin+vit_->var.pos[0].end);
                            ilen = it->var.pos[0].begin-vit_->var.pos[0].begin;
                            cvar.type = (0<dlen?sbio::DELETION:0)|sbio::INSERTION;
                            cvar.pos[0] = vit_->var.pos[0];
							cvar.pos[0].end = cvar.pos[0].begin + dlen - 1;
                            cvar.pos[1] = vit_->var.pos[0];
							cvar.pos[1].end = cvar.pos[1].begin + ilen - 1;
                        }
                        else {
                            dlen = it->var.pos[0].begin-vit_->var.pos[0].begin;
                            ilen = (it->var.pos[0].begin+it->var.pos[0].end) - (vit_->var.pos[0].begin+vit_->var.pos[0].end);
                            cvar.type = (0<dlen?sbio::DELETION:0)|sbio::INSERTION;
                            cvar.pos[0] = vit_->var.pos[0];
							cvar.pos[0].end = cvar.pos[0].begin + dlen - 1;
                            cvar.pos[1] = vit_->var.pos[0];
							cvar.pos[1].begin = vit_->var.pos[0].end;
							cvar.pos[1].end = cvar.pos[1].begin + ilen - 1;
                        }
                        cvar.alt = vit_->var.alt + "/" + it->var.alt;
                        it->var = cvar;
                        it->idx2 = vit_->idx2;
                        vit_->var.type = 0;
                    }
                }
                else {
                    if (it->var.pos[0].begin <= vit_->var.pos[0].begin &&
                        it->var.pos[0].begin+it->var.pos[0].end <= vit_->var.pos[0].begin+vit_->var.pos[0].end &&
                        vit_->var.pos[0].begin < it->var.pos[0].begin+it->var.pos[0].end) {
                        svar_data cvar;
                        int dlen, ilen;
                        if (it->var.type&sbio::DELETION) {
                            dlen = vit_->var.pos[0].begin-it->var.pos[0].begin;
							ilen = (vit_->var.pos[0].begin + vit_->var.pos[0].end) - (it->var.pos[0].begin + it->var.pos[0].end);
                            cvar.type = (0<dlen?sbio::DELETION:0)|sbio::INSERTION;
                            cvar.pos[0] = it->var.pos[0];
							cvar.pos[0].end = cvar.pos[0].begin + dlen - 1;
                            cvar.pos[1] = it->var.pos[0];
                            cvar.pos[1].begin = it->var.pos[0].end;
							cvar.pos[1].end = cvar.pos[1].begin + ilen - 1;
                        }
                        else {
							dlen = (vit_->var.pos[0].begin + vit_->var.pos[0].end) - (it->var.pos[0].begin + it->var.pos[0].end);
							ilen = vit_->var.pos[0].begin - it->var.pos[0].begin;
							cvar.type = (0 < dlen ? sbio::DELETION : 0) | sbio::INSERTION;
                            cvar.pos[0] = it->var.pos[0];
                            cvar.pos[0].begin = it->var.pos[0].end;
							cvar.pos[0].end = cvar.pos[0].begin + dlen - 1;
                            cvar.pos[1] = it->var.pos[0];
							cvar.pos[1].end = cvar.pos[1].begin + ilen - 1;
                        }
                        cvar.alt = it->var.alt + "/" + vit_->var.alt;
                        it->var = cvar;
                        it->idx2 = vit_->idx2;
                        vit_->var.type = 0;
                    }
                }
            }
            else if (it->var.type&sbio::TRANSLOCATION && vit_->var.type&sbio::TRANSLOCATION &&
                     it->var.pos[0].idx == vit_->var.pos[1].idx && it->var.pos[1].idx == vit_->var.pos[0].idx &&
                     it->var.pos[0].dir == vit_->var.pos[1].dir && it->var.pos[1].dir == vit_->var.pos[0].dir) {
                if (it->var.type&sbio::INVERSION) {
                    if(it->var.pos[0].dir) {
                        if (vit_->var.pos[1].begin < it->var.pos[0].begin &&
                            it->var.pos[1].begin <= vit_->var.pos[0].begin) {
                            svar_data cvar;
                            int dlen = it->var.pos[0].begin-vit_->var.pos[1].begin-1,
                            ilen = vit_->var.pos[0].begin-it->var.pos[1].begin+1;
                            cvar.type = (0<dlen?sbio::DELETION:0)|sbio::TRANSLOCATION|sbio::INSERTION|sbio::INVERSION;
                            cvar.pos[0] = vit_->var.pos[1];
                            ++cvar.pos[0].begin;
							cvar.pos[0].end = cvar.pos[0].begin + dlen - 1;
                            cvar.pos[1] = it->var.pos[0];
							cvar.pos[1].end = cvar.pos[1].begin + ilen - 1;
                            cvar.alt = vit_->var.alt + "/" + it->var.alt;
                            it->var = cvar;
                            it->idx2 = vit_->idx2;
                            vit_->var.type = 0;
                        }
                    }
                    else {
                        if (it->var.pos[0].begin < vit_->var.pos[1].begin &&
                            vit_->var.pos[0].begin <= it->var.pos[1].begin) {
                            svar_data cvar;
							int dlen = vit_->var.pos[1].begin - it->var.pos[0].begin - 1,
								ilen = it->var.pos[1].begin - vit_->var.pos[0].begin + 1;
                            cvar.type = (0<dlen?sbio::DELETION:0)|sbio::TRANSLOCATION|sbio::INSERTION|sbio::INVERSION;
                            cvar.pos[0] = it->var.pos[0];
                            ++cvar.pos[0].begin;
							cvar.pos[0].end = cvar.pos[0].begin + dlen - 1;
                            cvar.pos[1] = vit_->var.pos[0];
							cvar.pos[1].end = cvar.pos[1].begin + ilen - 1;
                            cvar.alt = it->var.alt + "/" + vit_->var.alt;
                            it->var = cvar;
                            it->idx2 = vit_->idx2;
                            vit_->var.type = 0;
                        }
                    }
                }
                else {
                    if(it->var.pos[0].dir) {
                        if (vit_->var.pos[1].begin < it->var.pos[0].begin &&
                            vit_->var.pos[0].begin <= it->var.pos[1].begin) {
                            svar_data cvar;
							int dlen = it->var.pos[0].begin - vit_->var.pos[1].begin - 1,
								ilen = it->var.pos[1].begin - vit_->var.pos[0].begin + 1;
                            cvar.type = (0<dlen?sbio::DELETION:0)|sbio::TRANSLOCATION|sbio::INSERTION;
                            cvar.pos[0] = vit_->var.pos[1];
                            ++cvar.pos[0].begin;
							cvar.pos[0].end = cvar.pos[0].begin + dlen - 1;
                            cvar.pos[1] = vit_->var.pos[0];
							cvar.pos[1].end = cvar.pos[1].begin + ilen - 1;
                            cvar.alt = vit_->var.alt + "/" + it->var.alt;
                            it->var = cvar;
                            it->idx2 = vit_->idx2;
                            vit_->var.type = 0;
                        }
                    }
                    else {
                        if (it->var.pos[0].begin < vit_->var.pos[1].begin &&
                            it->var.pos[1].begin <= vit_->var.pos[0].begin) {
                            svar_data cvar;
							int dlen = vit_->var.pos[1].begin - it->var.pos[0].begin - 1,
								ilen = vit_->var.pos[0].begin - it->var.pos[1].begin + 1;
                            cvar.type = (0<dlen?sbio::DELETION:0)|sbio::TRANSLOCATION|sbio::INSERTION;
                            cvar.pos[0] = it->var.pos[0];
                            ++cvar.pos[0].begin;
							cvar.pos[0].end = cvar.pos[0].begin + dlen - 1;
                            cvar.pos[1] = it->var.pos[1];
							cvar.pos[1].end = cvar.pos[1].begin + ilen - 1;
                            cvar.alt = it->var.alt + "/" + vit_->var.alt;
                            it->var = cvar;
                            it->idx2 = vit_->idx2;
                            vit_->var.type = 0;
                        }
                    }
                }
            }
            else if (it->var.type==sbio::INVERSION && vit_->var.type==sbio::INVERSION &&
                     it->var.pos[0].idx == vit_->var.pos[0].idx &&
                     it->var.pos[0].dir == vit_->var.pos[1].dir) {
                if (it->var.pos[0].dir &&
                    vit_->var.pos[1].begin < it->var.pos[0].begin && it->var.pos[1].begin <= vit_->var.pos[0].begin) {
                    svar_data cvar;
					int dlen = it->var.pos[0].begin - vit_->var.pos[1].begin,
						ilen = vit_->var.pos[0].begin - it->var.pos[1].begin + 1;
                    cvar.type = (0<dlen?sbio::DELETION:0)|sbio::INVERSION|sbio::INSERTION;
                    cvar.pos[0] = vit_->var.pos[1];
                    ++cvar.pos[0].begin;
					cvar.pos[0].end = cvar.pos[0].begin + dlen - 1;
                    cvar.pos[1] = it->var.pos[1];
					cvar.pos[1].end = cvar.pos[1].begin + ilen - 1;
                    cvar.alt = vit_->var.alt + "/" + it->var.alt;
                    it->var = cvar;
                    it->idx2 = vit_->idx2;
                    vit_->var.type = 0;
                }
                else if (!it->var.pos[0].dir &&
                    it->var.pos[0].begin < vit_->var.pos[1].begin && vit_->var.pos[0].begin <= it->var.pos[1].begin) {
                    svar_data cvar;
					int dlen = vit_->var.pos[1].begin - it->var.pos[0].begin,
						ilen = it->var.pos[1].begin - vit_->var.pos[0].begin + 1;
                    cvar.type = (0<dlen?sbio::DELETION:0)|sbio::INVERSION|sbio::INSERTION;
                    cvar.pos[0] = it->var.pos[0];
                    ++cvar.pos[0].begin;
					cvar.pos[0].end = cvar.pos[0].begin + dlen - 1;
                    cvar.pos[1] = vit_->var.pos[0];
					cvar.pos[1].end = cvar.pos[1].begin + ilen - 1;
                    cvar.alt = it->var.alt + "/" + vit_->var.alt;
                    it->var = cvar;
                    it->idx2 = vit_->idx2;
                    vit_->var.type = 0;
                }
            }
        }
    }
}

void findVar(va_result &vr, SBQuery *que, va_param *par) {
    auto rit = vr.aligns.begin();
    sforeach(vr.aligns) {
        findSmallVar(it-vr.aligns.begin(), E_, &que->query(it->ref.dir?1:0), vr.svariants, par);
        if (rit < it) findLargeVariant(rit-vr.aligns.begin(), it-vr.aligns.begin(),
                                        *rit, E_, vr.length, &que->query(0), vr.lvariants, par);
        rit = it;
    }
    if (!vr.lvariants.empty()) findComplexVariant(vr.lvariants);
}

void filterVar(va_result &vr, va_param *par) {
    if (!vr.lvariants.empty()) {
        size_t size = vr.lvariants.size();
        sforeach(vr.lvariants) {
            if (!E_.var.type) { --size; continue; }
            if ((E_.var.pos[0].length() < par->min_size && E_.var.pos[1].length() < par->min_size) ||
                !par->isTargeted(&it->var)) {
                E_.var.type = 0;
                --size; continue;
            }
        }
        std::sort(vr.lvariants.begin(), vr.lvariants.end(),
                  [](const va_pair &vp1, const va_pair &vp2){
                      if (!vp1.var.type) return false;
                      if (!vp2.var.type) return true;
                      return vp1.idx1 < vp2.idx1;
                  });
        vr.lvariants.resize(size);
    }
}
