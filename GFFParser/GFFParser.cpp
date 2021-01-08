#include "sbioinfo/sgf.h"
#include "sbioinfo/sbioseq.h"
#include "sapp/scuiapp.h"

using namespace slib;
using namespace slib::sapp;
using namespace slib::sbio;

class GFFParser : public SCuiApp {
public:
    int exec() {
        try {
            SGFFFile gff;
            SDictionary dict;
            gff.open(preference["_args"][0]);
			if (preference["out"] == "_STD_OUT_") STDIO_MODE;
			else FILEIO_MODE(preference["out"]);
            while (gff.next()) {
                auto &dat = gff.data();
                auto &arr = dict[dat.source][dat.type];
                sforeach(dat.attribute) {
                    if (arr.isArray() && arr.contain(E_.key)) continue;
                    else arr.add(E_.key);
                }
            }
            sforeach(dict) {
                auto keys = E_.value.keyset();
				sforeach_(kit, keys) SPrint(E_.key, TAB, *kit, TAB, E_.value[*kit]);
            }
        } 
		catch (SBioInfoException be) { be.print(); return 1; }
		catch (SIOException ie) { ie.print(); return 1; }
		catch (SException ex) { ex.print(); return 1; }
        return 0;
    }
public:
    GFFParser() : SCuiApp(
    {
        kv("app", V({
            kv("type", SCUI_APP | SINGLE_COMMAND),
            kv("name", "GFFParser"),
            kv("version", "1.2.0"),
            kv("creator", "Yuji Suehiro"),
            kv("license", "MIT license."),
            kv("develop", "2015/03/15"),
        })),
        kv("command", V({
            kv("_exec", V({
                kv("require", V({ "_args" }))
            })),
        })),
        kv("option", V({
            kv("_args", V({
                kv("caption", "input(.gff3)"),
                kv("description", "Input gff3 file.")
            })),
			kv("out", V({
				kv("short", "o"),
				kv("caption", "dest"),
				kv("description", "To save result, set the output file path. Or, set '_STD_OUT_' to display the result on the console.")
			}))
        }))
    }) {}
    ~GFFParser() {}
};

int main(int argc, const char **argv) {
    GFFParser app;
    return app.run(argc, argv);
}
