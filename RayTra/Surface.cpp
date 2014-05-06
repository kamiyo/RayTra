/*
 * Surface.cpp
 *
 *  Created on: Dec 2, 2011
 *      Author: kamiyo
 */

#include "Surface.h"

//Surface::Surface() {
//	// TODO Auto-generated constructor stub
//
//}
//
//Surface::~Surface() {
//	// TODO Auto-generated destructor stub
//}

bool Surface::_hit(Ray& ray, double t0, double t1, hitRecord& rec) {
	if (_trans) {
		Vector3d oEye = ray.eye;
		Vector3d oDir = ray.dir;
		Vector3d oInv = ray.inv;
		std::vector<int> oSign = ray.sign;
		ray.eye = apply(_mInv, oEye, 1);
		ray.dir = apply(_mInv, oDir, 0);
		ray.reSign();
		bool temp = hit(ray, t0, t1, rec);
		ray.eye = oEye;
		ray.dir = oDir;
		ray.inv = oInv;
		ray.sign = oSign;
		if (temp) {
			rec.n = apply(_mInvTrans, rec.n, 0);
		}
		return temp;
	}
	else {
		return hit(ray, t0, t1, rec);
	}
}

void Surface::trans(Matrix4d& inv) {
	_trans = true;
	_mInv = inv;
	_mInvTrans = inv.transpose();
}

Box Surface::combine(const Box& b1, const Box& b2) {
	Box b;
	b.MAX = b1.MAX.cwiseMax(b2.MAX);
	b.MIN = b1.MIN.cwiseMin(b2.MIN);
	return b;
}

int Surface::type() {
	return _type;
}
