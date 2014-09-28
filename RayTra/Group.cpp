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

void Group::addSurface(Surface* s) {
	_s.push_back(s);
	_b = _b.combine(s->_b);
}

bool Group::hit(RayBase& ray, double t0, double t1, hitRecord& rec) {
	bool hit = false;
	for (int i = 0; i < (int)_s.size(); i++) {
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
