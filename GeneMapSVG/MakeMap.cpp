#include "MakeMap.h"

MakeMap::MakeMap() {
	_width = 0;
	_scale = 0;
	_minsize = 100;
}
MakeMap::~MakeMap() {}

void MakeMap::openDB(const char *path) { _db.open(path); }
void MakeMap::setPos(sbpos& pos) { 
	_pos = pos;
	setScale();
}
void MakeMap::resize(size_t width) { 
	_width = width;
	setScale();
}
void MakeMap::setScale() { 
	if (_width) {
		if (_minsize < _pos.length(true)) _scale = (float)_width / _pos.length(true);
		else _scale = (float)_width / _minsize;
	}
	else _scale = 0;
}
sfig MakeMap::_makeLine(sbio::annot_info* info, float h, const SColor& col) {
	float left = _scale * (info->begin - _pos.begin),
		right = _scale * (info->end - _pos.begin);
	if (right < left + 1.0f) right = left + 1.0f;
	SLine2D line(v2f(left, h), v2f(right, h));
	line.setStrokeColor(col);
	return line;
}
sfig MakeMap::_makeBox(sbio::annot_info* info, float h, const SColor& col) {
	float width = _scale * info->length(true);
	if (width < 1.0f) width = 1.0f;
	SRectangle rect(_scale * (info->begin - _pos.begin), h, width, 5.0f);
	rect.setFillColor(col);
	return rect;
}
sfig MakeMap::_makeTriangle(int start, bool dir, float h, const SColor& col) {
	float init = _scale * (start - _pos.begin);
	SPolygon poly;
	poly.addVertex(v2f(init, h - 2.5f));
	poly.addVertex(v2f(init, h + 2.5f));
	poly.addVertex(v2f(init + (dir ? -5.0f : 5.0f), h));
	poly.setFillColor(col);
	return poly;
}

sfig MakeMap::chromosome() {
	sfig chr(sshape::GROUP);
	chr->attribute()["name"] = "chromosome";
	sline2d chr_line(v2f(0.0f, 20.0f), v2f((float)_width, 20.0f));
	chr_line->setStrokeWidth(2.0f);
	scalligraphy chr_label(0, 12.0, _db.chrInfo(_pos.idx).name);
	chr_label->setFont("Arial", 12.0f);
	chr->addFigure(chr_line);
	chr->addFigure(chr_label);
	sfig chr_scale(sshape::GROUP);
	SPath2D scales;
	sint lunit = log(_pos.length(true)) / log(10.0);
	String unit = "";
	sint unit_len = 1;
	if (8 < lunit) {
		unit = "G"; unit_len = power(10, 9);
	}
	else if (5 < lunit) {
		unit = "M"; unit_len = power(10, 6);
	}
	else if (2 < lunit) {
		unit = "K"; unit_len = power(10, 3);
	}
	if (lunit) {
		lunit = power(10, lunit);
		sint sunit = lunit / 10;
		auto init = _pos.begin / lunit;
		init *= lunit;
		while (init < _pos.end) {
			sline2d ls(v2f((init - _pos.begin) * _scale, 12.5f), v2f((init - _pos.begin) * _scale, 27.5f));
			ls->setStrokeWidth(1.5f);
			scales.addFigure(ls);
			scalligraphy scale_label((init - _pos.begin) * _scale - 2.5f, 10.0, String(init / unit_len) + unit);
			scale_label->setFont("Arial", 9.0f);
			if(!chr_label->cross(scale_label)) chr_scale->addFigure(scale_label);
			init += lunit;
		}

		init = _pos.begin / sunit;
		init *= sunit;
		while (init < _pos.end) {
			sline2d ls(v2f((init - _pos.begin) * _scale, 16.0f), v2f((init - _pos.begin) * _scale, 24.0f));
			ls->setStrokeWidth(0.75f);
			scales.addFigure(ls);
			init += sunit;
		}
		chr_scale->addFigure(scales);
	}
	chr->addFigure(chr_scale);
    return chr;
}
inline void shift(sfig fig) {
	if (fig->childCount() < 2) return;
    bool move = true;
    while (move) {
        move = false;
        auto it = fig->begin();
        while (it < fig->end()-1) {
            auto it_ = it+1;
            while (it_ < fig->end()) {
                if (E_->boundary().overlap((*it_)->boundary())) {
                    move = true;
					(*it_)->shift(v2f(0.0f, E_->boundary().height + 25.0f));
                }
                else if (E_->boundary().ori_x+E_->boundary().width < (*it_)->boundary().ori_x) break;
                ++it_;
            }
            NEXT_;
        }
    }
}
sfig MakeMap::genes() {
    SBAnnotDB::geneparray array;
    _db.geneInfo(array, _pos, false);
    sfig gene_area(sshape::GROUP);
	gene_area->attribute()["name"] = "genes";
    if (!array.empty()) {
		sforeach(array) {
            sfig gene_g(sshape::GROUP);
			gene_g->attribute()["name"] = E_->gene_id;
            auto gbox = _makeBox(E_, 15.0f, color::NAVY);
			scalligraphy gene_label(gbox->boundary().ori_x, 12.0f, E_->name);
			if (gbox->boundary().ori_x < 0.0) gene_label->shift(v2f(-gbox->boundary().ori_x, 0));
			gene_g->addFigure(gbox);
            gene_g->addFigure(gene_label);
            gene_area->addFigure(gene_g);
        }
        shift(gene_area);
    }
	return gene_area;
}
sfig MakeMap::transcripts() {
    SBAnnotDB::geneparray array;
    _db.geneInfo(array, _pos);
    sfig trs_area(sshape::GROUP);
	trs_area->attribute()["name"] = "transcripts";
    if (!array.empty()) {
        sforeach(array) {
			if (E_->transcripts.empty()) continue;
			sfig gene_g(sshape::GROUP);
			gene_g->attribute()["name"] = E_->name;
			int h = 0;
			sforeach_(tit, E_->transcripts) {
				if (_pos.end < (*tit)->begin || (*tit)->end < _pos.begin) continue;
				sfig trs_g(sshape::GROUP);
				trs_g->attribute()["name"] = (*tit)->name;
				auto line = _makeLine(*tit, h + 15.0f, color::BLACK);
				auto head = _makeTriangle((E_->dir ? (*tit)->begin : (*tit)->end), E_->dir, h + 15.0f, color::BLACK);
				scalligraphy trs_label(0.0f, h + 10.0f, (*tit)->name);
				if (E_->dir) {
					if (_width < line->boundary().ori_x + line->boundary().width)
						trs_label->shift(v2f(_width - trs_label->boundary().width, 0));
					else trs_label->shift(v2f(line->boundary().ori_x + line->boundary().width - trs_label->boundary().width, 0));
				}
				else if (0.0 <= line->boundary().ori_x && line->boundary().ori_x < _width)
					trs_label->shift(v2f(line->boundary().ori_x, 0));
				trs_label->setFont("Arial", 10.0f);
				trs_g->addFigure(trs_label);
				trs_g->addFigure(line);
				trs_g->addFigure(head);
				if (E_->type & PROTEN_CODING && (*tit)->type & M_RNA) {
					auto mes = (*tit)->messenger();
					sforeach_(sit, mes) {
						sfig sbox;
						if (sit->type & CDS) sbox = _makeBox(&(*sit), h + 12.5f, E_->dir ? color::SPRING : color::MAGENTA);
						else sbox = _makeBox(&(*sit), h + 12.5f, color::WHITE);
						trs_g->addFigure(sbox);
					}
				}
				else {
					sforeach_(sit, (*tit)->structures) {
						if (sit->type & EXON) {
							auto sbox = _makeBox(&(*sit), h + 12.5f, E_->dir ? color::DARKGREEN : color::DARKRED);
							trs_g->addFigure(sbox);
						}
					}
				}
				gene_g->addFigure(trs_g);
                h += 20.0f;
            }
            trs_area->addFigure(gene_g);
        }
        shift(trs_area);
    }
	return trs_area;
}
sfig MakeMap::mutations() {
    SBAnnotDB::mutparray array;
    _db.mutantInfo(array, _pos);
    sfig mut_area(sshape::GROUP);
	mut_area->attribute()["name"] = "mutants";
    if (!array.empty()) {
        sforeach(array) {
            sfig mut_g(sshape::GROUP);
			mut_g->attribute()["name"] = E_->name;
            auto mbox = _makeBox(E_, 15.0f, color::RED);
			scalligraphy mut_label(mbox->boundary().ori_x, 12.0f, E_->name);
			if (mbox->boundary().ori_x < 0.0) mut_label->shift(v2f(-mbox->boundary().ori_x, 0));
            mut_g->addFigure(mbox);
            mut_g->addFigure(mut_label);
            mut_area->addFigure(mut_g);
        }
        shift(mut_area);
    }
	return mut_area;
}
sfig MakeMap::features(intarray& types) {
	SBAnnotDB::ftrparray array;
    _db.featureInfo(array, _pos);
    sfig ftr_area(sshape::GROUP);
	ftr_area->attribute()["name"] = "features";
	if (!array.empty()) {
		sforeach(array) {
			if (types.find(E_->type) == NOT_FOUND) continue;
            sfig ftr_g(sshape::GROUP);
			ftr_g->attribute()["name"] = E_->name;
			auto fbox = _makeBox(E_, 15.0f, color::BROWN);
			scalligraphy ftr_label(fbox->boundary().ori_x, 12.0f, E_->name);
			if (fbox->boundary().ori_x < 0.0) ftr_label->shift(v2f(-fbox->boundary().ori_x, 0));
			ftr_g->addFigure(fbox);
            ftr_g->addFigure(ftr_label);
            ftr_area->addFigure(ftr_g);
        }
        shift(ftr_area);
    }
	return ftr_area;
}