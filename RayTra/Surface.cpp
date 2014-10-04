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

bool Surface::_hit(RayBase& ray, double t0, double t1, hitRecord& rec) const  {
	if (_trans) {
		RayBase tRay(apply(_mInv, ray.m_eye, 1), apply(_mInv, ray.m_dir, 0), ray.m_type);
		bool temp = hit(tRay, t0, t1, rec);
		if (temp) {
			rec.n = apply(_mTrans, rec.n, 0);
			rec.n.normalize();
		}
		return temp;
	}
	else {
		return hit(ray, t0, t1, rec);
	}
}

void Surface::trans(Matrix4d& m, Matrix4d& inv) {
	_trans = true;
	_mInv = inv;
	_mTrans = m.inverse().transpose();
	_b = _b.transform(m);
}

int Surface::type() {
	return _type;
}
