#ifndef MOIRAI_DBMAKER_H
#define MOIRAI_DBMAKER_H

#include "snet/snet.h"
#include "sbioinfo/sbannot.h"
#include "sbioinfo/sgf.h"
#include "sbioinfo/sbioseq.h"

using namespace slib;
using namespace slib::sbio;

typedef enum {
    INFO = 0,
    CHROMOSOME = 1,
    CONTIG = 2,
    GENE = 3,
    TRANSCRIPT = 4,
    STRUCTURE = 5,
    MUTANT = 6,
    VARIATION = 7,
    FEATURE = 8,
} table_id;


extern const char* TABLE_NAMES[10] = {
	"INFO",
	"CHROMOSOME",
	"CONTIG",
	"GENE",
	"TRANSCRIPT",
	"STRUCTURE",
	"MUTATION",
	"VARIATION",
	"FEATURE"
};

extern void initTables(STable *tables);
extern void makeInfoTable(STable *table, SDictionary &dict);
extern void makeChrTable(STable *table, SBSeqList *ref);

//-----------------H.sapiens----------------------
extern void makeHumanDB(STable *table, SDictionary &dict, SBSeqList *ref);

//------------------------------------------------

//-----------------M.musculus---------------------
extern void makeMouseDB(STable *table, SDictionary &dict, SBSeqList *ref);

//------------------------------------------------

//------------------D.rerio-----------------------
extern void makeFishDB(STable *table, SDictionary &dict, SBSeqList *ref);

//------------------------------------------------

//---------------D.melanogaster-------------------
extern void makeFlyDB(STable *table, SDictionary &dict, SBSeqList *ref);

//------------------------------------------------

//-----------------C.elegans----------------------
extern void makeWormDB(STable *table, SDictionary &dict, SBSeqList *ref);

//------------------------------------------------

#endif
