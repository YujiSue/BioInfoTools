#ifndef MOIRAI_MAKEMAP_H
#define MOIRAI_MAKEMAP_H

#include "smedia/sfigure.h"
#include "sbioinfo/sbannot.h"

using namespace slib;
using namespace slib::smedia;
using namespace slib::sbio;

class MakeMap {
private:
    SBAnnotDB _db;
	size_t _width, _minsize;
    float _scale;
    sbpos _pos;

private:
    sfig _makeLine(sbio::annot_info *info, float h, const SColor &col);
    sfig _makeBox(srange *info, float h, const SColor &col);
    sfig _makeTriangle(int start, bool dir, float h, const SColor &col);

public:
	MakeMap();
    ~MakeMap();
    
    void openDB(const char *path);

	void setPos(sbpos &pos);
	void resize(size_t width);
	void setScale();
	
    sfig chromosome();
	sfig genes();
	sfig transcripts();
	sfig mutations();
	sfig variants();
	sfig features(intarray &types);
	sfig customs(SArray &info);
};
#endif
