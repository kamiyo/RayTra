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
		Ray tRay(apply(_mInv, ray.eye, 1), apply(_mInv, ray.dir, 0), ray.ref, ray.alpha, Ray::VIEW);
		Ray::count -= 1;
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

bool Surface::_hit(Photon& photon, double t0, double t1, hitRecord& rec) {
	if (_trans) {
		Photon tPhoton(apply(_mInv, photon.m_pos, 1), apply(_mInv, photon.m_dir, 0), photon.m_intensity, photon.m_color);
		Photon::count -= 1;
		bool temp = hit(tPhoton, t0, t1, rec);
		if (temp) {
			rec.n = apply(_mTrans, rec.n, 0);
			rec.n.normalize();
		}
		return temp;
	}
	else {
		return hit(photon, t0, t1, rec);
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
