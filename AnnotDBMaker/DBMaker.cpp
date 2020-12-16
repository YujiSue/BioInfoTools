#include "DBMaker.h"

void initTables(STable *tables) {
	tables[0] = STable({
		kv("columns", V({
			V({ kv("type", INTEGER_COLUMN | sql::KEY_COLUMN), kv("name", "ID") }),
			V({ kv("type", STRING_COLUMN), kv("name", "NAME") }),
			V({ kv("type", STRING_COLUMN), kv("name", "VALUE") })
			}))
		});
	tables[1] = STable({
		kv("columns", V({
			V({ kv("type", INTEGER_COLUMN | sql::KEY_COLUMN), kv("name", "ID") }),
			V({ kv("type", STRING_COLUMN), kv("name", "NAME") }),
			V({ kv("type", STRING_COLUMN), kv("name", "LENGTH") })
			}))
		});
	tables[2] = STable({
		kv("columns", V({
			V({ kv("type", INTEGER_COLUMN | sql::KEY_COLUMN), kv("name", "ID") }),
			V({ kv("type", STRING_COLUMN), kv("name", "NAME") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "CHROMOSOME") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "START") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "END") })
			}))
		});
	tables[3] = STable({
		kv("columns", V({
			V({ kv("type", INTEGER_COLUMN | sql::KEY_COLUMN), kv("name", "ID") }),
			V({ kv("type", STRING_COLUMN), kv("name", "GENE_ID") }),
			V({ kv("type", STRING_COLUMN), kv("name", "NAME") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "TYPE") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "CHROMOSOME") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "START") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "END") }),
			V({ kv("type", STRING_COLUMN), kv("name", "STRAND") }),
			V({ kv("type", STRING_COLUMN), kv("name", "OTHER_NAME") }),
			V({ kv("type", ARRAY_COLUMN), kv("name", "TRANSCRIPT") }),
			V({ kv("type", STRING_COLUMN), kv("name", "DESCRIPTION") }),
			V({ kv("type", DICT_COLUMN), kv("name", "ATTRIBUTE") })
			}))
		});
	tables[4] = STable({
		kv("columns", V({
			V({ kv("type", INTEGER_COLUMN | sql::KEY_COLUMN), kv("name", "ID") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "GENE_ID") }),
			V({ kv("type", STRING_COLUMN), kv("name", "NAME") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "TYPE") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "CHROMOSOME") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "START") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "END") }),
			V({ kv("type", ARRAY_COLUMN), kv("name", "STRUCTURE") })
			}))
		});
	tables[5] = STable({
		kv("columns", V({
			V({ kv("type", INTEGER_COLUMN | sql::KEY_COLUMN), kv("name", "ID") }),
			V({ kv("type", ARRAY_COLUMN), kv("name", "TRANSCRIPT_ID") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "TYPE") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "CHROMOSOME") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "START") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "END") })
			}))
		});
	tables[6] = STable({
		kv("columns", V({
			V({ kv("type", INTEGER_COLUMN | sql::KEY_COLUMN), kv("name", "ID") }),
			V({ kv("type", STRING_COLUMN), kv("name", "MUT_ID") }),
			V({ kv("type", STRING_COLUMN), kv("name", "NAME") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "TYPE") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "CHROMOSOME") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "START") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "END") }),
			V({ kv("type", STRING_COLUMN), kv("name", "STRAIN") }),
			V({ kv("type", STRING_COLUMN), kv("name", "ATTRIBUTE") })
			}))
		});
	tables[7] = STable({
		kv("columns", V({
			V({ kv("type", INTEGER_COLUMN | sql::KEY_COLUMN), kv("name", "ID") }),
			V({ kv("type", STRING_COLUMN), kv("name", "VAR_ID") }),
			V({ kv("type", STRING_COLUMN), kv("name", "NAME") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "TYPE") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "CHROMOSOME") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "START") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "END") }),
			V({ kv("type", STRING_COLUMN), kv("name", "STRAIN") }),
			V({ kv("type", STRING_COLUMN), kv("name", "ATTRIBUTE") })
			}))
		});
	tables[8] = STable({
		kv("columns", V({
			V({ kv("type", INTEGER_COLUMN | sql::KEY_COLUMN), kv("name", "ID") }),
			V({ kv("type", STRING_COLUMN), kv("name", "NAME") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "TYPE") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "CHROMOSOME") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "START") }),
			V({ kv("type", INTEGER_COLUMN), kv("name", "END") })
			}))
		});
}
void makeInfoTable(STable *table, SDictionary &dict) {
    table->addRow({ snull, "creator", dict["creator"] });
    table->addRow({ snull, "date", SDate(sstyle::YMD) });
    table->addRow({ snull, "version", dict["dbver"] });
    table->addRow({ snull, "species", dict["species"] });
	auto others = dict["attribute"].parse(";", "=");
	sforeach(others) {
		table->addRow({ snull, E_.key, E_.value });
	}
}
void makeChrTable(STable *table, SBSeqList *ref) {
    sforeachi(*ref) table->addRow({ i, ref->at(i)->name, ref->at(i)->length()});
}