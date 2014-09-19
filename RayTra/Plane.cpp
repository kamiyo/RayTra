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
	__m256d e_v = _load4d(ray.eye);
	__m256d d_v = _load4d(ray.dir);
	__m256d n_v = _load4d(_n);
	double den = _dot(n_v, d_v);
	if (den == 0) {
		return false;
	}
	else {
		double num = _dot(n_v, _mm256_sub_pd(_load4d(_p), e_v));
		rec.t = num / den;
		if (rec.t < t0 || rec.t > t1) {
			return false;
		}
		else {
			if (ray.type == Ray::VIEW) {
				rec.n = _store4d(n_v).normalized();
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
