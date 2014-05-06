/*
 * Group.cpp
 *
 *  Created on: Dec 2, 2011
 *      Author: kamiyo
 */

#include "Group.h"

Group::Group() {
	Box b;
	_b = b;

}

void Group::addSurface(Surface* s) {
	_s.push_back(s);
	_b = combine(_b, s->_b);
}

bool Group::hit(Ray& ray, double t0, double t1, hitRecord& rec) {
	bool hit = false;
	for (int i = 0; i < (int)_s.size(); i++) {
		hitRecord temp;
//		std::cout << ray.eye << std::endl;
		if (_s[i]->hit(ray, t0, t1, temp)) {
			hit = true;
			rec = temp;
			t1 = rec.t;
		}
	}
	return hit;
}

void Group::boundingBox() {
}

Group::~Group() {
	// TODO Auto-generated destructor stub
}
