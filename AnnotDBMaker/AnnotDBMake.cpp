#include "sapp/scuiapp.h"
#include "DBMaker.h"

using namespace slib;
using namespace slib::sbio;

typedef int makeDB(STable* tables, SDictionary& dict, SBSeqList* ref);

class AnnotDBMake : public sapp::SCuiApp {
    SDataBase db;
    SBSeqList reference;
    STable tables[9];
private:
    int exec() {
        try {
            sio::SFile dbfile(preference["out"]);
            if (dbfile.exist()) dbfile.remove();
            db.open(dbfile);
            initTables(tables);
            
            makeInfoTable(&tables[INFO], preference);
            db.createTable("INFO", tables[INFO]);
            
            reference.makeIndex(preference["reference"]);
            makeChrTable(&tables[CHROMOSOME], &reference);
            db.createTable("CHROMOSOME", tables[CHROMOSOME]);

			sapp::SPlugIn<STable *, SDictionary &, SBSeqList *> plugin(preference["program"], "makeDB");
			plugin.exec(tables, preference, &reference);
            sforin(i, 2, 9) db.createTable(DB_TABLE_NAMES[i], tables[i]);
            
        } catch (sio::SIOException ie) {
            ie.print();
            return 1;
        } catch (SDBException de) {
            de.print();
            return 1;
        } catch (SBioInfoException be) {
            be.print();
            return 1;
        } catch (sapp::SAppException ae) {
			ae.print();
			return 1;
		}
        return 0;
    }
    
public:
    AnnotDBMake() : sapp::SCuiApp(
    {
        kv("app", V({
            kv("type", sapp::SCUI_APP|sapp::SINGLE_COMMAND),
            kv("name", "AnnotDBMake"),
            kv("version", "1.1.0"),
            kv("creator", "Yuji Suehiro"),
            kv("license", "MIT license."),
            kv("develop", "2015/02/21"),
        })),
        kv("command", V({
            kv("_exec", V({
                kv("require", V({ "reference", "program", "out" }))
            }))
        })),
        kv("option", V({
			kv("reference", V({
				kv("short", "r"),
				kv("caption", "file"),
				kv("description", "Reference file prepared by 'GenomeConverter'.")
			})),
			kv("program", V({
				kv("short", "p"),
				kv("caption", "prog."),
				kv("description", "Dynamic library(.so/.dll) to make database for each species.")
			})),
			kv("gff", V({
				kv("short", "g"),
				kv("caption", "file"),
				kv("description", "GFF3 file.")
			})),
			kv("out", V({
				kv("short", "o"),
				kv("caption", "output"),
				kv("description", "Output file path.")
			})),
			kv("dbver", V({
				kv("short", "d"),
                kv("caption", "version"), 
				kv("description", "Database version.")
            })),
            kv("species", V({
				kv("short", "s"),
                kv("caption", "name"),
				kv("description", "Name of the species.")
            })),
			kv("creator", V({
				kv("short", "c"),
				kv("caption", "creator"),
				kv("description", "Creator's name.")
			})),
			kv("attribute", V({
				kv("short", "a"),
                kv("caption", "attribute"), 
				kv("description", "Database attribute (key1=value1;key2=value2;...).")
            })),
            kv("extra", V({
				kv("short", "e"),
                kv("caption", "files"),
				kv("description", "Extra data resource(tag1:file1,tag2:file2,...).")
            }))
        }))
    }) {}
};

int main(int argc, const char * argv[]) {
    AnnotDBMake app;
    return app.run(argc, argv);
}
