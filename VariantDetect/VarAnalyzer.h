#ifndef MOIRAI_VARANALYZER
#define MOIRAI_VARANALYZER

#define ANALYSIS_COMPLETED 0x01
#define SHORT_SEQUENCE_ERROR 0x02
#define NOT_ALIGNED_ERROR 0x04
#define TOO_MUCH_N_ERROR 0x08
#define NO_VARIANT_ERROR 0x10
#define NO_SAMPLE_ERROR 0x20
#define MULTI_HIT_ERROR 0x40

#include "sutil/sdb.h"
#include "sbioinfo/sbsearch.h"
#include "sbioinfo/svariant.h"

using namespace slib;
using namespace slib::sio;
using namespace slib::sbio;

struct va_param {
    sbsearch_param search_par;
    bool show_align;
    int min_size, sv_penalty;
    
    SBSeqList ref;
    Array<sregion> target;
    
    va_param() {
        show_align = false;
        min_size = 1;
        sv_penalty = 0;
    }
    ~va_param() {}
    
    void loadRange(const char *path) {
        if(ref.empty()) return;
        target.resize(ref.size());
        try {
            SString str;
            str.load(path);
            auto list = str.splitline();
            sforeach(list) {
                auto row = E_.split("\t");
                target[ref.seqIdx(row[0])].add(srange(row[1].intValue(), row[2].intValue()));
            }
        }
        catch(SIOException ie) { ie.print(); return; }
    }
    
    bool isTargeted(svar_data *var) {
        if (target.empty()) return true;
        if (!(var->type&sbio::INSERTION) && var->type&sbio::TRANSLOCATION) {
            return target[var->pos[0].idx].include(srange(var->pos[0].begin, var->pos[0].begin)) ||
            target[var->pos[1].idx].include(srange(var->pos[1].begin, var->pos[1].begin));
        }
        else return target[var->pos[0].idx].include(srange(var->pos[0].begin, var->pos[0].end));
    }
};

extern srange arrange(salign *a, size_t &len);

struct align_map {
    size_t length;
    Array<std::pair<srange, salign *>> aligns;
    
    align_map();
    align_map(size_t l);
    align_map(const align_map &am);
    ~align_map();
    
    align_map &operator=(const align_map &am);
    size_t append_len(salign *a);
    void append(salign *a);
    bool operator<(const align_map &am) const;
    bool operator==(const align_map &am) const;
};

struct va_pair {
    int idx1, idx2;
    svar_data var;
    
    va_pair();
    va_pair(int i, svar_data &v);
    va_pair(int i1, int i2, svar_data &v);
    ~va_pair();
    
    bool operator<(const va_pair &vap) const;
    bool operator==(const va_pair &vap) const;
};

struct va_result {
    size_t length;
    Array<salign> aligns;
    int total_score;
    Array<va_pair> svariants, lvariants;
    
    va_result();
    va_result(const va_result &result);
    ~va_result();
    
    va_result &operator=(const va_result &result);
    bool operator<(const va_result &res) const;
    bool operator==(const va_result &res) const;
};

extern void findVar(va_result &vr, SBQuery *que, va_param *par);
extern void filterVar(va_result &vr, va_param *par);

class VarAnalyzer {
private:
    va_param *_par;
    SBQuery _que;
    SBSearch _sbs;
    SBExtend _sbe;
    Array<salign *> _aligns;
    Array<align_map> _maps;
    size_t _seqlen;
    
public:
    Array<va_result> results;
    
public:
    VarAnalyzer(va_param &par);
    ~VarAnalyzer();
    
    int analyze(sobj obj);
    void reset();
};

#endif
