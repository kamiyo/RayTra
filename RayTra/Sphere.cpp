/*
 * Sphere.cpp
 *
 *  Created on: Dec 2, 2011
 *      Author: kamiyo
 */

#include "Sphere.h"

Sphere::Sphere(Vector3d pos, double rad, Material* m) {
	_p = pos;
	_r = rad;
	_m = m;
	boundingBox();
	_l = NULL;
	_type = SPHERE;
	_r2 = _r * _r;
}

Sphere::Sphere(LightP* l) {
	_p = l->_pos;
	_r = l->_r;
	_m = NULL;
	boundingBox();
	_l = l;
	_type = SPHERE;
}

Sphere::~Sphere() {
	// TODO Auto-generated destructor stub
}

bool Sphere::hit(Ray& ray, double t0, double t1, hitRecord& rec) {
	Vector3d e = ray.eye;
	Vector3d d = ray.dir;
	Vector3d ep = e - _p;
	double dep = d.dot(ep);
	double dd = d.dot(d);
	double disc = (dep * dep) - dd * (ep.dot(ep) - (_r2));
	if (disc < 0) {
		return false;
	}
	else {
		disc = sqrt(disc);
		rec.t = (-1.0 * dep - disc) / dd;
		if (rec.t > t1) {
			return false;
		}
		if (rec.t < t0) {
			rec.t = (-1.0 * dep + disc) / dd;

			if (rec.t < t0 || rec.t > t1) {
				return false;
			}
			if (ray.type == Ray::VIEW) {
				Vector3d n = e + rec.t * d;
				rec.n = (n - _p).normalized();
			} else if (ray.type == Ray::SHAD) {
				rec.s = this;
			}
			rec.m = _m;
			return true;
		}
		else {
			if (ray.type == Ray::VIEW) {
				Vector3d n = e + rec.t * d;
				rec.n = (n - _p).normalized();
			}
			else if (ray.type == Ray::SHAD) {
				rec.s = this;
			}
			rec.m = _m;
			return true;
		}
	}

}

void Sphere::boundingBox() {
	_b.set(_p.array() - _r, _p.array() + _r);
}