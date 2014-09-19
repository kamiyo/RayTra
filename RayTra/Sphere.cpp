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
	__m256d e_v = _load4d(ray.eye);
	__m256d p_v = _load4d(_p);
	__m256d d_v = _load4d(ray.dir);
	__m256d ep_v = _mm256_sub_pd(e_v, p_v);
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
				//Vector3d n = e + rec.t * d;
				__m256d n_v = _mm256_sub_pd(_mm256_fmadd_pd(_mm256_set1_pd(rec.t), d_v, e_v), p_v);
				//rec.n = (n - _p).normalized();
				__m256d n_norm = _dot_mm(n_v, n_v);
				rec.n = _store4d(_mm256_div_pd(n_v, n_norm));
			} else if (ray.type == Ray::SHAD) {
				rec.s = this;
			}
			rec.m = _m;
			return true;
		}
		else {
			if (ray.type == Ray::VIEW) {
				//Vector3d n = e + rec.t * d;
				__m256d n_v = _mm256_sub_pd(_mm256_fmadd_pd(_mm256_set1_pd(rec.t), d_v, e_v), p_v);
				//rec.n = (n - _p).normalized();
				__m256d n_norm = _dot_mm(n_v, n_v);
				rec.n = _store4d(_mm256_div_pd(n_v, n_norm));
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