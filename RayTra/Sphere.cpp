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

	__m256d ep_v = _load4d(ep);
	__m256d d_v = _load4d(d);

	double dep = _dot(ep_v, d_v);
	double dd = _dot(d_v, d_v);
	double disc = (dep * dep) - dd * (_dot(ep_v, ep_v) - _r2);
	if (disc < 0) {
		return false;
	}
	else {
		disc = sqrt(disc);
		rec.t = (-1.0 * dep - disc) / dd;
		if (rec.t > t1) {
			return false;
		}
		if (rec.t > t1) return false;
		if (rec.t < t0) {
			rec.t = (-1.0 * dep + disc) / dd;

			if (rec.t < t0 || rec.t > t1) {
				return false;
			}
			if (ray.type == Ray::VIEW) {
				Vector3d n = e + rec.t * d;
				//__m256d n_v = _mm256_sub_pd(_mm256_add_pd(_load4d(e), _mm256_mul_pd(d_v, _mm256_set1_pd(rec.t))), _load4d(_p));
				rec.n = (n - _p).normalized();
				//rec.n = _store4d(n_v).normalized();
			} else if (ray.type == Ray::SHAD) {
				rec.s = this;
			}
			rec.m = _m;
			return true;
		}
		else {
			if (ray.type == Ray::VIEW) {
				Vector3d n = e + rec.t * d;
				//__m256d n_v = _mm256_sub_pd(_mm256_add_pd(_load4d(e), _mm256_mul_pd(d_v, _mm256_set1_pd(rec.t))), _load4d(_p));
				rec.n = (n - _p).normalized();
				//rec.n = _store4d(n_v).normalized();
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