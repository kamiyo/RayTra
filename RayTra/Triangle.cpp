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
	// TODO Auto-generated constructor stub

}

Triangle::~Triangle() {
	// TODO Auto-generated destructor stub
}

bool Triangle::hit(Ray& ray, double t0, double t1, hitRecord& rec) {
	Vector3d dir = ray.dir;
	Vector3d eye = ray.eye;
	Vector3d abc = _p1 - _p2;
	Vector3d def = _p1 - _p3;
	Vector3d ghi = dir;
	Vector3d jkl = _p1 - eye;
	Matrix3d T; T << abc, def, ghi;

	double M = T.determinant();
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
	if (ray.type == Ray::VIEW) {
		rec.t = t;
		rec.n = ((_p2 - _p1).cross(_p3 - _p1)).normalized();
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
