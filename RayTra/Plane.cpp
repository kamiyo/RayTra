/*
 * Plane.cpp
 *
 *  Created on: Dec 4, 2011
 *      Author: kamiyo
 */

#include "Plane.h"
#include <iostream>

Plane::Plane(Vector3d n, Vector3d p, Material* m) {
	_p = p;
	_n = n;
	_m = m;
	boundingBox();
	_type = PLANE;
}

Plane::~Plane() {
	// TODO Auto-generated destructor stub
}

bool Plane::hit(Ray& ray, double t0, double t1, hitRecord& rec) {
	Vector3d e = ray.eye;
	Vector3d d = ray.dir;
	double num = _n.dot(_p - e);
	double den = _n.dot(d);
	if (den == 0) {
		return false;
	} else {
		rec.t = num / den;
		if (rec.t < t0 || rec.t > t1) {
			return false;
		} else {
			if (ray.type != Ray::OCCL) {
				if (ray.type != Ray::SHAD) {
					if (d.dot(_n) > 0) {
						rec.n = -1.0 * _n.normalized();
					} else {
						rec.n = _n.normalized();
					}
					rec.m = _m;
				}
				rec.s = NULL;
			}
			return true;
		}
	}
}

void Plane::boundingBox() {
	_b.MAX = Vector3d(INF, INF, INF);
	_b.MIN = Vector3d(nINF, nINF, nINF);
}
