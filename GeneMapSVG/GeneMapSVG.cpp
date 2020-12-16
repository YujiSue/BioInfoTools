#include "sapp/scuiapp.h"
#include "sbioinfo/sbioseq.h"
#include "MakeMap.h"

using namespace slib;
using namespace slib::smath;
using namespace slib::smedia;
using namespace slib::sapp;
using namespace slib::sbio;

class MapSVG : public SCuiApp {
public:
	SCanvas canvas;
	SBSeqList ref;
	sbpos pos;
	MakeMap mapper;
	
public:
    int exec() {
        try {
            ref.makeIndex(preference["ref"]);
			mapper.openDB(preference["annot"]);
			mapper.resize(preference["width"]);
			pos = sbpos(preference["pos"], &ref.index);
			mapper.setPos(pos);
			auto chr = mapper.chromosome();
			canvas.drawFigure(chr);
			if (preference["gene"]) { 
				auto gene = mapper.genes();
				gene->shift(v2f(0.0f, canvas.root().boundary().ori_y + canvas.root().boundary().height + 20.0f));
				canvas.drawFigure(gene);
			}
			if (preference["trs"]) { 
				auto trs = mapper.transcripts();
				trs->shift(v2f(0.0f, canvas.root().boundary().ori_y + canvas.root().boundary().height + 20.0f));
				canvas.drawFigure(trs);
			}
			if (preference["mut"]) { 
				auto mut = mapper.mutations();
				mut->shift(v2f(0.0f, canvas.root().boundary().ori_y + canvas.root().boundary().height + 20.0f));
				canvas.drawFigure(mut);
			}
			if (preference["var"]) {
				auto var = mapper.variants();
				var->shift(v2f(0.0f, canvas.root().boundary().ori_y + canvas.root().boundary().height + 20.0f));
				canvas.drawFigure(var);
			}
			/*
			if (preference["ftr"]) {
				auto ftr = mapper.features();
				ftr->shift(v2f(0.0f, canvas.root().boundary().ori_y + canvas.root().boundary().height + 20.0f));
				canvas.drawFigure(ftr);
			}
			*/
			if (preference["custom"]) {
				auto list = String::dequot(preference["custom"]).split(",");
				SArray info(list.size());
				sforeach2(info, list) { E1_ = E2_.parse(";", "="); }
				auto cstm = mapper.customs(info);
				cstm->shift(v2f(0.0f, canvas.root().boundary().ori_y + canvas.root().boundary().height + 20.0f));
				canvas.drawFigure(cstm);
			}
			canvas.resize(preference["width"], canvas.root().boundary().ori_y + canvas.root().boundary().height + 5.0f);
			canvas.save(preference["out"]);
        } catch(SBioInfoException be) {
            be.print();
            return 1;
        }
        return 0;
    }
public:
    MapSVG() : SCuiApp(
    {
        kv("app", V({
			kv("type", sapp::SCUI_APP|sapp::SINGLE_COMMAND),
            kv("name", "MapSVG"),
            kv("version", "1.1.0"),
            kv("creator", "Yuji Suehiro"),
            kv("license", "MIT license."),
            kv("develop", "2019/02/13"),
        })),
        kv("command", V({
            kv("_exec", V({
                kv("require", V({ "ref", "annot", "pos", "width", "out" }))
            })),
        })),
		kv("option", V({
			kv("ref", V({
				kv("short", "r"),
				kv("caption", "file"),
				kv("description", "Reference file path.")
			})),
			kv("annot", V({
				kv("short", "a"),
				kv("caption", "file"),
				kv("description", "Annotation database file path.")
			})),
			kv("pos", V({
				kv("short", "p"),
				kv("caption", "position"),
				kv("description", "Position to obtain the map.")
			})),
			kv("out", V({
				kv("short", "o"),
				kv("caption", "file"),
				kv("description", "File path to export.")
			})),
			kv("gene", V({
				kv("short", "g"),
				kv("type", "bool"),
				kv("description", "Show genes positions.")
			})),
			kv("trs", V({
				kv("short", "t"),
				kv("type", "bool"),
				kv("description", "Show transcripts positions.")
			})),
			kv("mut", V({
				kv("short", "m"),
				kv("type", "bool"),
				kv("description", "Show mutations positions.")
			})),
			kv("var", V({
				kv("short", "v"),
				kv("type", "bool"),
				kv("description", "Show mutations positions.")
			})),
			/*
			kv("ftr", V({
				kv("short", "f"),
				kv("type", "bool"),
				kv("description", "Show features positions.")
			})),
			*/
			kv("custom", V({
				kv("caption", "information"),
				kv("description", "Show original features. \nInformation should be defined as [{name=NNN;pos=1:12345..23456;col=red}, ...]")
			})),
			kv("width", V({
				kv("short", "w"),
                kv("caption", "value"),
                kv("description", "Width of viewport.")
            }))
        }))
    }) {}
    ~MapSVG() {}
};
int main(int argc, const char * argv[]) {
    MapSVG app;
    return app.run(argc, argv);
}
