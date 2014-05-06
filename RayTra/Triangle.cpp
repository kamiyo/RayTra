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
	double a = _p1[0] - _p2[0]; double d = _p1[0] - _p3[0]; double g = dir[0]; double j = _p1[0] - eye[0];
	double b = _p1[1] - _p2[1]; double e = _p1[1] - _p3[1]; double h = dir[1]; double k = _p1[1] - eye[1];
	double c = _p1[2] - _p2[2]; double f = _p1[2] - _p3[2]; double i = dir[2]; double l = _p1[2] - eye[2];
	double eihf = e * i - h * f;
	double gfdi = g * f - d * i;
	double dheg = d * h - e * g;
	double akjb = a * k - j * b;
	double jcal = j * c - a * l;
	double blkc = b * l - k * c;
	double M = a * eihf + b * gfdi + c * dheg;
	double t = -1.0 * (f * akjb + e * jcal + d * blkc) / M;
	if (t < t0 || t > t1) {
		return false;
	}
	double gamma = (i * akjb + h * jcal + g * blkc) / M;
	if (gamma < 0 || gamma > 1) {
		return false;
	}
	double beta = (j * eihf + k * gfdi + l * dheg) / M;
	if (beta < 0 || beta > (1 - gamma)) {
		return false;
	}
	if (ray.type != Ray::OCCL && ray.type != Ray::SHAD) {
		rec.t = t;
		rec.n = ((_p2 - _p1).cross(_p3 - _p1)).normalized();
		rec.m = _m;
	}
	return true;
}

void Triangle::boundingBox() {

	_b.MAX = _p1.cwiseMax(_p2.cwiseMax(_p3));
	_b.MIN = _p1.cwiseMin(_p2.cwiseMin(_p3));

}
