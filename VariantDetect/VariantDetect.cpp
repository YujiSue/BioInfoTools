#include "sapp/scuiapp.h"
#include "VarAnalyzer.h"

using namespace slib;
using namespace slib::sapp;
using namespace slib::sbio;

class VariantDetect : public SCuiApp {
public:
	va_param par;

private:
	void _showResult(va_result &res) {
		auto& v1 = res.svariants;
		if (!v1.empty()) {
			sforeach_(vit, v1) {
				switch (vit->var.type) {
				case sbio::SNV:
					std::cout << par.ref[vit->var.pos[0].idx]->name << ":" << vit->var.pos[0].begin << " (" <<
						par.ref[vit->var.pos[0].idx]->raw(vit->var.pos[0].begin-1, 1) <<
						"->" << vit->var.alt << " substitution)." << std::endl;
					break;
				case sbio::MNV:
					std::cout << par.ref[vit->var.pos[0].idx]->name << ":" << vit->var.pos[0].begin << " (" <<
						par.ref[vit->var.pos[0].idx]->raw(vit->var.pos[0].begin-1, vit->var.alt.length()) <<
						"->" << vit->var.alt << " substitutions)." << std::endl;
					break;
				case sbio::DELETION:
					std::cout << par.ref[vit->var.pos[0].idx]->name << ":" << vit->var.pos[0].begin << "-" <<
						vit->var.pos[0].end << " (" << vit->var.pos[0].length() + 1 << " bp deletion)." << std::endl;
					break;
				case sbio::INSERTION:
					std::cout << par.ref[vit->var.pos[0].idx]->name << ":" << vit->var.pos[0].begin << "-" <<
						vit->var.alt << "-" << vit->var.pos[0].end << " (" << vit->var.alt.length() << " bp insertion)." << std::endl;
					break;
				default:
					break;
				}
			}
		}
		auto& v2 = res.lvariants;
		if (!v2.empty()) {
			sforeach_(vit, v2) {
				if (vit->var.type & sbio::INSERTION) {
					if (vit->var.type & sbio::TRANSLOCATION) {
						if (vit->var.type & sbio::INVERSION) {
							std::cout << par.ref[vit->var.pos[0].idx]->name << ":" << vit->var.pos[0].begin << "-<" <<
								par.ref[vit->var.pos[1].idx]->name << ":" << vit->var.pos[1].end << "-" << vit->var.pos[1].begin <<
								">-" << vit->var.pos[0].end << " (" << vit->var.pos[1].length() << " bp inverted translocational insertion)";
						}
						else {
							std::cout << par.ref[vit->var.pos[0].idx]->name << ":" << vit->var.pos[0].begin << "-<" <<
								par.ref[vit->var.pos[1].idx]->name << ":" << vit->var.pos[1].begin << "-" << vit->var.pos[1].end <<
								">-" << vit->var.pos[0].end << " (" << vit->var.pos[1].length() << " bp translocational insertion)";
						}
					}
					else if (vit->var.type & sbio::INVERSION) {
						std::cout << par.ref[vit->var.pos[0].idx]->name << ":" << vit->var.pos[0].begin << "-<" <<
							par.ref[vit->var.pos[1].idx]->name << ":" << vit->var.pos[1].end << "-" << vit->var.pos[1].begin <<
							">-" << vit->var.pos[0].end << " (" << vit->var.pos[1].length() << " bp inverted insertion)";
					}
					else if (vit->var.type & sbio::DELETION) {
						std::cout << par.ref[vit->var.pos[0].idx]->name << ":" << vit->var.pos[0].begin << "-<" <<
							par.ref[vit->var.pos[1].idx]->name << ":" << vit->var.pos[1].begin << "-" << vit->var.pos[1].end <<
							">-" << vit->var.pos[0].end << " (" << vit->var.pos[0].length() << " deletion + " << vit->var.pos[1].length() << " bp insertion)";
					}
					else {
						std::cout << par.ref[vit->var.pos[0].idx]->name << ":" << vit->var.pos[0].begin << "-<" <<
							par.ref[vit->var.pos[1].idx]->name << ":" << vit->var.pos[1].begin << "-" << vit->var.pos[1].end <<
							">-" << vit->var.pos[0].end << " (" << vit->var.pos[1].length() << " bp insertion)";
					}
				}
				else {
					if (vit->var.type & sbio::TRANSLOCATION) {
						if (vit->var.type & sbio::INVERSION) {
							std::cout << par.ref[vit->var.pos[0].idx]->name << ":" << vit->var.pos[0].begin << (vit->var.pos[0].dir ? "(-)" : "(+)") << "-" <<
								par.ref[vit->var.pos[1].idx]->name << ":" << vit->var.pos[1].begin << (vit->var.pos[1].dir ? "(-)" : "(+)") << " (translocational inversion)." << std::endl;
						}
						else {
							std::cout << par.ref[vit->var.pos[0].idx]->name << ":" << vit->var.pos[0].begin << "-" <<
								par.ref[vit->var.pos[1].idx]->name << ":" << vit->var.pos[1].begin << " (translocation)." << std::endl;
						}
					}
					else if (vit->var.type & sbio::INVERSION) {
						std::cout << par.ref[vit->var.pos[0].idx]->name << ":" << vit->var.pos[0].begin << (vit->var.pos[0].dir ? "(-)" : "(+)") << "-" <<
							par.ref[vit->var.pos[1].idx]->name << ":" << vit->var.pos[1].begin << (vit->var.pos[1].dir ? "(-)" : "(+)") << " (inversion)." << std::endl;
					}
					else if (vit->var.type & sbio::DELETION) {
						std::cout << par.ref[vit->var.pos[0].idx]->name << ":" << vit->var.pos[0].begin << "-" <<
							vit->var.pos[0].end << " (" << vit->var.pos[0].length() + 1 << " bp deletion)." << std::endl;
					}
					else if (vit->var.type & sbio::DUPLICATION) {
						std::cout << par.ref[vit->var.pos[0].idx]->name << ":" << vit->var.pos[0].begin << "-" <<
							vit->var.pos[0].end << " (" << vit->var.pos[0].length() + 1 << " bp duplication)." << std::endl;
					}
				}
			}
		}
	}

public:
    int exec() {
        try {
			par.ref.load(preference["ref"]);
			if (preference["target"]) par.loadRange(preference["target"]);
			par.search_par.setType(par.ref[0]->type());
			if (preference.hasKey("gap")) par.search_par.max_gap = preference["gap"];
			if (preference.hasKey("miss")) par.search_par.max_miss = preference["miss"];
			if (preference.hasKey("match")) par.search_par.min_match = preference["match"];
			if (preference.hasKey("seed")) par.search_par.setSeed(preference["seed"]);
			if (preference.hasKey("threshold")) par.search_par.extend_threshold = preference["threshold"];
            VarAnalyzer analyzer(par);
			auto files = preference["input"].split(",");
            sforeach(files) {
                auto res = analyzer.analyze(E_);
                switch (res) {
                    case NO_SAMPLE_ERROR:
                        std::cout<<"Sequence file is not found."<<std::endl;
                        break;
                    case SHORT_SEQUENCE_ERROR:
                        std::cout<<"Sequence is too short."<<std::endl;
                        break;
                    case TOO_MUCH_N_ERROR:
                        std::cout<<"There are too much N to analyze."<<std::endl;
                        break;
                    case NOT_ALIGNED_ERROR:
                         std::cout<<"Sequence is not aligned to the reference."<<std::endl;
                        break;
                    case NO_VARIANT_ERROR:
                        std::cout<<"There is no variant."<<std::endl;
                        break;
                    case MULTI_HIT_ERROR:
					{
						std::cout << "There are multiple candidates." << std::endl;
						sforeach_(rit, analyzer.results) {
							_showResult(analyzer.results[0]);
							if (rit != analyzer.results.end() - 1) std::cout << String("*") * 60 << std::endl;
							std::cout << std::endl;
						}
						break;
					}
                    default:
						_showResult(analyzer.results[0]);
						break;
                }
                analyzer.reset();
            }
        } catch (SBioInfoException be) {
            be.print(); return 1;
        }
        return 0;
    }
    
public:
    VariantDetect() : SCuiApp(
    {
        kv("app", V({
			kv("type", sapp::SCUI_APP | sapp::SINGLE_COMMAND),
            kv("name", "VariantDetect"),
            kv("version", "1.1.0"),
            kv("creator", "Yuji Suehiro"),
            kv("license", "MIT license."),
            kv("develop", "2015/02/21"),
        })),
        kv("command", V({
            kv("_exec", V({
                kv("require", V({ "ref", "input" }))
            }))
        })),
        kv("option", V({
            kv("ref", V({
				kv("short", "r"),
                kv("caption", "file"),
                kv("description", "Reference file path.")
            })),
			kv("input", V({
				kv("short", "i"),
				kv("caption", "file(fa/txt/seq/ab1)"),
				kv("description", "Input file path.")
			})),
            kv("gap", V({
				kv("short", "g"),
                kv("type", "num"),
                kv("caption", "size"),
				kv("description", "Permissible gap size.")
            })),
            kv("match", V({
				kv("short", "m"),
                kv("type", "num"),
                kv("caption", "size"), 
				kv("description", "Minimum match size.")
            })),
            kv("seed", V({
				kv("short", "s"),
                kv("type", "num"),
                kv("caption", "size"),
				kv("description", "Seed size.")
            })),
            kv("threshold", V({
				kv("short", "t"),
                kv("type", "num"),
                kv("caption", "value"),
				kv("description", "Threshold to extend alignemnt.")
            }))
			/*,
            kv("align", V({
                kv("short", "a"),
				kv("type", "bool"), 
				kv("description", "Show alignment(s).")
            }))
			*/
        }))
    }) {}
    ~VariantDetect() {}
};

int main(int argc, const char * argv[]) {
    VariantDetect vd;
    return vd.run(argc, argv);
}
