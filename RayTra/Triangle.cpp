/*
 * Triangle.cpp
 *
 *  Created on: Dec 4, 2011
 *      Author: kamiyo
 */

#include "Triangle.h"

Triangle::Triangle(Vector3d p1, Vector3d p2, Vector3d p3, Material* m) {
	_p1 = p1;
	_p2 = p2;
	_p3 = p3;
	_m = m;
	boundingBox();
	_type = TRIANGLE;
	abc = _p1 - _p2;
	def = _p1 - _p3;
	_n = ((-abc).cross(-def)).normalized();

	//Plucker
	_u1 = _p2 - _p1;
	_u2 = _p3 - _p2;
	_u3 = _p1 - _p3;
	_v1 = _p2.cross(_p1);
	_v2 = _p3.cross(_p2);
	_v3 = _p1.cross(_p3);
}

Triangle::~Triangle() {
	// TODO Auto-generated destructor stub
}


// TO-DO: PLUCKER COORDS?
bool Triangle::hit(Ray& ray, double t0, double t1, hitRecord& rec) {
	//plucker
#ifdef _PLUCKER_
	Vector3d e = ray.eye;
	Vector3d u = ray.dir;
	Vector3d v = ray.v;
	double alpha = _u1.dot(v) + u.dot(_v1);
	double beta = _u2.dot(v) + u.dot(_v2);
	if ((alpha < 0) != (beta < 0)) return false;
	double gamma = _u3.dot(v) + u.dot(_v3);
	if ((alpha < 0) != (gamma < 0)) return false;

	if ((alpha == beta) && (alpha == gamma) && alpha == 0) return false;
	double sum = alpha + beta + gamma;
	
	alpha /= sum; beta /= sum; gamma /= sum;
	Vector3d p = _p1 * beta + _p2 * gamma + _p3 * alpha;
	double t = (p - e).norm() / u.norm();

#else
	Vector3d ghi = ray.dir;
	Vector3d jkl = _p1 - ray.eye;
	Matrix3d T; T << abc, def, ghi;

	double M = T.determinant();
	if (M == 0) return false;
	Matrix3d A; A << def, abc, jkl;
	double t = -1.0 * A.determinant() / M;
	if (t < t0 || t > t1) {
		return false;
	}
	Matrix3d B; B << ghi, abc, jkl;
	double gamma = B.determinant() / M;
	if (gamma < 0 || gamma > 1) {
		return false;
	}
	Matrix3d C; C << jkl, def, ghi;
	double beta = C.determinant() / M;
	if (beta < 0 || beta > (1 - gamma)) {
		return false;
	}
#endif
	if (ray.type == Ray::VIEW) {
		rec.t = t;
		rec.n = _n;
		//if (_n.dot(u) > 0) rec.n = -_n;
	}
	else if (ray.type == Ray::SHAD) {
		rec.s = this;
	}
	rec.m = _m;
	return true;
}

void Triangle::boundingBox() {
	_b.set(_p1.cwiseMin(_p2.cwiseMin(_p3)), _p1.cwiseMax(_p2.cwiseMax(_p3)));
}
