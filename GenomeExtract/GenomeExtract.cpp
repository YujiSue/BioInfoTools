#include "sapp/scuiapp.h"
#include "sbioinfo/sbioseq.h"

using namespace slib;
using namespace slib::sio;
using namespace slib::sbio;
using namespace slib::sapp;

class GenomeExtract : public SCuiApp {
public:
    int exec() {
        try {
            SBSeqList ref;
            ref.load(preference["ref"]);
            sforeach(preference["_args"]) {
                sbpos pos((const char *)E_, &ref.index); // string -> sbpos
                pos.shift(-1); // to zero-based
				if (preference["rev"]) pos.dir = true;
                auto seq = ref.getSeq(DNA_SEQ1, pos);
                seq.name = E_;
				if (!preference["format"] || preference["format"] == "txt")
					std::cout << ref[pos.idx]->raw(pos) << std::endl;
				else if (preference["format"] == "fa") {
					std::cout << ">" << seq.name << std::endl;
					auto i = 0, l = pos.length() + 1;
					while (i + FASTA_ROW_CHAR < l) {
						std::cout << ref[pos.idx]->raw(pos.begin + i, FASTA_ROW_CHAR) << std::endl;
						i += FASTA_ROW_CHAR;
					}
					std::cout << ref[pos.idx]->raw(pos.begin + i, l - i) << std::endl;
				}
				else if (preference["format"] == "gbk") {
					/*
					 */
				}
            }
        } catch (SBioInfoException be) {
            be.print();
            return 1;
        }
        return 0;
    }
    
public:
    GenomeExtract() : SCuiApp(
    {
        kv("app", V({
            kv("type", SCUI_APP | SINGLE_COMMAND),
            kv("name", "GenomeExtract"),
            kv("version", "1.1.0"),
            kv("creator", "Yuji Suehiro"),
            kv("license", "MIT license."),
            kv("develop", "2015/02/21")
        })),
        kv("command", V({
			kv("_exec", V({
				kv("require", V({ "ref", "_args" }))
			}))
        })),
        kv("option", V({
            kv("ref", V({
				kv("short", "r"),
                kv("caption", "file"),
                kv("description", "Reference file path.")
            })),
			/*
            kv("annot", V({
				kv("short", "a"),
                kv("caption", "file"),
                kv("description", "Annotation database file path.")
            })),
			*/
            kv("format", V({
				kv("short", "f"),
                kv("caption", "txt/fa"),
				//kv("caption", "txt/fa/gbk"),
                kv("description", "File format to display or export.")
            })),
			kv("rev", V({
				kv("type", "bool"),
				kv("description", "Get reverse strand seq.")
			})),
			/*
			kv("out", V({
				kv("short", "o"),
				kv("caption", "file"),
				kv("description", "File path to export.")
			})),
			*/
            kv("_args", V({
                kv("caption", "position(s)"), kv("multi", true),
                kv("description", "Position to obtain the sequence in the following format; 'Ref:Start..End', 'Ref:Start-End', 'Ref,Start,End' or 'Ref[TAB]Start[TAB]End'.")
            }))
        }))
    }) {}
    ~GenomeExtract() {}
};

int main(int argc, const char **argv) {
    GenomeExtract app;
    return app.run(argc, argv);
}
