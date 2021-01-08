#include "sapp/scuiapp.h"
#include "sbioinfo/sbannot.h"
#include "sbioinfo/svarutil.h"

using namespace slib;
using namespace slib::sio;
using namespace slib::sbio;
using namespace slib::sapp;

#define TYPE_CHR 0x01
#define TYPE_CTG 0x02
#define TYPE_GENE 0x10
#define TYPE_TRANSCRIPT 0x20
#define TYPE_MUTATION 0x40
#define TYPE_FETURE 0x80
#define OUT_GENENAME 0x00
#define OUT_GENEID 0x01

class BioAnnot : public SCuiApp {
private:
    SBAnnotDB _annot;
	bool _tsite;
	subyte _from, _to, _gtype;
    
	inline void convertType(sobj obj, subyte &byte) {
		if (obj == "pos") byte = TYPE_CHR;
		else if (obj == "ctg") byte = TYPE_CTG;
		else if (obj == "gene") byte = TYPE_GENE;
		else if (obj == "trs") byte = TYPE_TRANSCRIPT;
		else if (obj == "mut") byte = TYPE_MUTATION;
		else if (obj == "ftr") byte = TYPE_FETURE;
		else byte = 0;
	}

#define TYPE_CTG 0x02
#define TYPE_GENE 0x10
#define TYPE_TRANSCRIPT 0x20
#define TYPE_MUTATION 0x40
#define TYPE_FETURE 0x80

    int annotation() {
        try {
			Array<sbpos> posarray;
			if (preference["in"]) {

			}
			if (preference["out"] && preference["out"] != "_STD_OUT_") FILEIO_MODE(preference["out"]);
			sforeach(preference["_args"]) {
				SPrint(E_, TAB, ">>", TAB);
				if (_from == _to) {
					SPrint(E_);
					continue;
				}
				switch (_from) {
				case TYPE_CHR:
					posarray.add(sbpos(E_, &_annot.nameIdx()));
					break;
				case TYPE_CTG:
				{
					SBAnnotDB::ctgparray array;
					_annot.ctgInfo(array, E_);
					sforeach_(pit, array) posarray.add(**pit);
					break;
				}
				case TYPE_GENE:
				{
					SBAnnotDB::geneparray array;
					_annot.geneInfo(array, E_);
					sforeach_(pit, array) posarray.add(**pit);
					break;
				}
				case TYPE_TRANSCRIPT:
				{
					SBAnnotDB::trsparray array;
					_annot.transcriptInfo(array, E_);
					sforeach_(pit, array) posarray.add(**pit);
					break;
				}
				case TYPE_MUTATION:
				{
					SBAnnotDB::mutparray array;
					_annot.mutantInfo(array, E_);
					sforeach_(pit, array) posarray.add(**pit);
					break;
				}
				case TYPE_FETURE:
				{
					SBAnnotDB::ftrparray array;
					_annot.featureInfo(array, E_);
					sforeach_(pit, array) posarray.add(**pit);
					break;
				}
				}
				switch (_to) {
				case TYPE_CHR:
				{
					sforeach_(pit, posarray) {
						SPrint(_annot.chrInfo(pit->idx).name, ":", pit->begin, "..", pit->end, "(", (pit->dir ? "-" : "+"), ")");
					}
					break;
				}
				case TYPE_CTG:
				{
					SBAnnotDB::ctgparray array;
					sforeach_(pit, posarray) {
						_annot.ctgInfo(array, *pit);
						if (!array.empty()) {
							SWrite(array.first()->name, ":", pit->begin - array.first()->begin + 1);
							if (pit->end <= array.first()->end) SPrint(pit->end - array.first()->begin + 1);
							else  SPrint(array.last()->name, ":", pit->end - array.last()->begin + 1);
						}
						else SPrint("Not found.");
						array.clear();
					}
					break;
				}
				case TYPE_GENE:
				{
					String str;
					SBAnnotDB::geneparray array;
					sforeach_(pit, posarray) {
						_annot.geneInfo(array, *pit);
						if (!array.empty()) {
							sforeach_(git, array) {
								if (_gtype == OUT_GENEID) str << (*git)->gene_id << ",";
								else str << (*git)->name << ",";
							}
							str.resize(str.size() - 1);
						}
						else str << "Not found.";
						SPrint(str );
						array.clear(); str.clear();
					}
					break;
				}
				case TYPE_TRANSCRIPT:
				{
					String str;
					SBAnnotDB::trsparray array;
					sforeach_(pit, posarray) {
						_annot.transcriptInfo(array, *pit);
						if (!array.empty()) {
							sforeach_(tit, array) { 
								str << (*tit)->name;
								if (_tsite) {
									sushort _site = 0;
									sforeach_(sit, (*tit)->structures) {
										if (sit->overlap(*pit)) _site |= sit->type;
									}
									auto gsites = sbiutil::geneSite(_site);
									str << "(" << (gsites.empty()?String("") : gsites[0]) << ")";
								}
								str << ",";
							}
							str.resize(str.size() - 1);
						}
						else str << "Not found.";
						SPrint(str);
						array.clear(); str.clear();
					}
					break;
				}
				case TYPE_MUTATION:
				{
					String str;
					SBAnnotDB::mutparray array;
					sforeach_(pit, posarray) {
						_annot.mutantInfo(array, *pit);
						if (!array.empty()) {
							sforeach_(mit, array) str << (*mit)->name << ",";
							str.resize(str.size() - 1);
						}
						else str << "Not found.";
						SPrint(str);
						array.clear(); str.clear();
					}
					break;
				}
				case TYPE_FETURE:
				{
					String str;
					SBAnnotDB::ftrparray array;
					sforeach_(pit, posarray) {
						_annot.featureInfo(array, *pit);
						if (!array.empty()) {
							sforeach_(fit, array) str << (*fit)->name << ",";
							str.resize(str.size() - 1);
						}
						else str << "Not found.";
						SPrint(str);
						array.clear(); str.clear();
					}
					break;
				}
				}
				posarray.clear();
			}
        } catch (SBioInfoException be) {
            be.print();
            return 1;
        }
        return 0;
    }
    int exec() {
        try {
            _annot.open(preference["db"]);
			convertType(preference["from"], _from);
			convertType(preference["to"], _to);
			if (!_from || !_to) {
				SPrint("Please select a type from 'pos, gene, trs, or mut' for 'from' and 'to' options.");
				return 1;
			}
			if (preference["gtype"]) {
				if (preference["gtype"] == "id") _gtype = OUT_GENEID;
				else if (preference["gtype"] == "name") _gtype = OUT_GENENAME;
				else _gtype = 0;
			}
			if (preference["tsite"]) _tsite = true;
			else _tsite = false;
			return annotation();
        } catch (sio::SIOException ie) {
            ie.print(); return 1;
        } catch (SBioInfoException be) {
            be.print(); return 1;
        }
    }
    
public:
    BioAnnot() : SCuiApp(
    {
        kv("app", V({
            kv("type", SCUI_APP|SINGLE_COMMAND),
            kv("name", "BioAnnot"),
            kv("version", "1.1.0"),
            kv("creator", "Yuji Suehiro"),
            kv("license", "MIT license."),
            kv("develop", "2015/02/21")
        })),
        kv("command", V({
            kv("_exec", V({
                kv("require", V({ "db", "from", "to", "_args" }))
				kv("select", V({"_args", "in"}))
            }))
        })),
        kv("option", V({
            kv("db", V({
				kv("short", "d"), 
				kv("caption", "database"),
                kv("description", "Annotation database file path.")
            })),
            kv("from", V({
                kv("short", "f"), 
				kv("caption", "source(pos/gene/trs/mut)"),
                kv("description", "Genomic position([Chr]:[Start]..[End]), Gene name, Transcript name, or Mutant name.")
            })),
            kv("to", V({
                kv("short", "t"), 
				kv("caption", "output(pos/gene/trs/mut)"),
                kv("description", "Genomic position([Chr]:[Start]..[End]), Gene name, Transcript name, or Mutant name.")
            })),
			kv("gtype", V({
				kv("short", "g"),
				kv("caption", "id/name"),
				kv("description", "Type of gene output(Unique gene ID, or Gene name.")
			})),
			kv("tsite", V({
				kv("short", "s"),
				kv("type", "bool"),
				kv("description", "Show sites of transcripts (CDS/exon/intron/5'UTR/3'UTR).")
			})),
			kv("in", V({
				kv("short", "i"),
				kv("caption", "file"),
				kv("description", "Input file")
			})),
			kv("out", V({
				kv("short", "o"),
				kv("caption", "file"),
				kv("description", "Output file")
			})),
			kv("_args", V({
				kv("caption", "input"),
				kv("description", "Input argment")
			}))
        }))
    }) {}
    ~BioAnnot() {}
};

int main(int argc, const char * argv[]) {
    BioAnnot app;
    return app.run(argc, argv);
}
