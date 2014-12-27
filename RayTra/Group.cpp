/*
 * Group.cpp
 *
 *  Created on: Dec 2, 2011
 *      Author: kamiyo
 */

#include "Group.h"

Group::Group() {
	BBox b;
	_b = b;

}

void Group::addSurface(u_ptr<Surface>& s) {
	_b.combine(s->_b);
	_s.push_back(std::move(s));
}

void Group::renderBoundingBox(std::vector<std::vector<float> >& verts, int level) const {
	for (size_t i = 0; i < _s.size(); i++) {
		_s[i]->renderBoundingBox(verts, level);
	}
}


bool Group::hit(RayBase& ray, double t0, double t1, hitRecord& rec) const {
	bool hit = false;
	for (size_t i = 0; i < _s.size(); i++) {
		hitRecord temp;
		if (_s[i]->_hit(ray, t0, t1, temp)) {
			hit = true;
			rec = temp;
			t1 = rec.t;
		}
	}
	if (hit) {
		if (rec.t < t0 || rec.t > t1) return false;
	}
	return hit;
}

void Group::boundingBox() {
}

Group::~Group() {
	// TODO Auto-generated destructor stub
}
