#include "sapp/scuiapp.h"
#include "sbioinfo/sbioinfo.h"

using namespace slib;
using namespace slib::sio;
using namespace slib::sbio;

class GenomeConverter : public sapp::SCuiApp {
public:
    int exec() {
        try {
			SBSeqList reference;
            sforeach(preference["_args"]) {
				std::cout << "loading '" << E_ << "'" << std::endl;
				sio::SFile file(E_);
				if (file.filename().beginWith("GCF_")) preference["refseq"] = true;
				SBSeqIO::makeIndex(sbio::ATTRIBUTED | sbio::MASKED | sbio::DNA_SEQ4, file, &reference);
				if (preference.hasKey("species")) reference.attribute["species"] = preference["species"];
				if (preference.hasKey("refver")) reference.attribute["version"] = preference["refver"];
				if (preference.hasKey("source")) reference.attribute["source"] = preference["source"];
            }
            if (preference["refseq"]) {
				auto size = reference.size();
				sforeach(reference) {
					E_->attribute["_origin"] = E_->name;
					auto list = E_->name.split(" ");
					if (E_->name.beginWith("NC")) {
						if (E_->name.contain("mito")) E_->name = "mtDNA";
						else {
							auto name = list[list.find("chromosome") + 1];
							if (name.endWith(",")) name.resize(name.length() - 1);
							E_->name = name;
						}
					}
					else {
						E_->name = list[0];
						if (preference["linkage"]) --size;
					}
				}
				reference.sort([](const sbseq& s1, const sbseq& s2) {
					if (s1->name.beginWith("N")) return false;
					if (s2->name.beginWith("N")) return true;
					if (s1->name == "X") {
						if (s2->name == "Y" || s2->name == "mtDNA") return true;
						else return false;
					}
					if (s1->name == "Y") {
						if (s2->name == "mtDNA") return true;
						else return false;
					}
					if (s1->name == "mtDNA") return false;
					if (s2->name == "X") {
						if (s1->name == "Y" || s1->name == "mtDNA") return false;
						return true;
					}
					if (s2->name == "Y") {
						if (s1->name == "mtDNA") return false;
						return true;
					}
					if (s2->name == "mtDNA") return true;
					return s1->name.intValue() < s2->name.intValue();
				});
				if (preference["linkage"]) {
					SArray tmp;
					sforin(i, 0, size) {
						auto idx = reference.index[reference[i]->attribute["_origin"]];
						tmp.add(reference.attribute["_offset"][idx]);
					}
					reference.attribute["_offset"].array().swap(tmp);
					reference.resize(size);
					reference.index.clear();
					reference.lengthList().clear();
					sforeachi(reference) {
						reference.index[reference[i]->name] =  i;
						reference.lengthList().add(reference[i]->length());
					}
				}
            }
			std::cout << "exporting... '" << preference["out"] << "'" << std::endl;
			reference.save(preference["out"]);
            std::cout<<"Completed."<<std::endl;
        } catch (SException se) {
            se.print();
            return 1;
        } catch (SIOException ie) {
            ie.print();
            return 1;
        } catch (SBioInfoException be) {
            be.print();
            return 1;
        }
        return 0;
    }
    
public:
    GenomeConverter() : sapp::SCuiApp(
    {
        kv("app", V({
            kv("type", sapp::SCUI_APP | sapp::SINGLE_COMMAND),
            kv("name", "GenomeConverter"),
            kv("version", "1.1.0"),
            kv("creator", "Yuji Suehiro"),
            kv("develop", "2015/02/21"),
            kv("license", "MIT license."),
        })),
        kv("command", V({
            kv("_exec", V({
                kv("require", V({ "_args", "out" }))
            })),
        })),
        kv("option", V({
            kv("_args", V({
                kv("caption", "input(s)"),
                kv("description", "Reference genome file(s) in FASTA format.")
            })),
			kv("out", V({
				kv("short", "o"),
				kv("caption", "'output'"),
				kv("description", "Output file path.")
			})),
			kv("species", V({
				kv("short", "s"),
				kv("caption", "name"),
				kv("description", "Species.")
			})),
			kv("refver", V({
				kv("short", "r"),
				kv("caption", "version"),
				kv("description", "Reference version.")
			})),
			/*
            kv("source", V({
				kv("short", "u"),
                kv("caption", "url"),
                kv("description", "Source URL.")
            })),
			*/
			kv("linkage", V({
				kv("short", "l"),
				kv("type", "bool"),
				kv("description", "Select only linkaged sequence.")
			}))
        }))
    }) {}
    ~GenomeConverter() {}
};

int main(int argc, const char * argv[]) {
	GenomeConverter app;
    return app.run(argc, argv);
}