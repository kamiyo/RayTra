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
	double den = _n.dot(d);
	if (den == 0) {
		return false;
	}
	else {
		double num = _n.dot(_p - e);
		rec.t = num / den;
		if (rec.t < t0 || rec.t > t1) {
			return false;
		}
		else {
			if (ray.type == Ray::VIEW) {
				rec.n = _n.normalized();
				rec.m = _m;
			}
			else {
				rec.s = this;
			}
			return true;
		}
	}
}

void Plane::boundingBox() {
}
