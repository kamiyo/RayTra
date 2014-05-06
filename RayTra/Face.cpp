/*
 * face.cpp
 *
 *  Created on: Oct 19, 2011
 *      Author: kamiyo
 */

#include "Face.h"

Face::Face(Material* m) {
	_m = m;
	_normal.setZero();
	smooth = false;
	boundingBox();
	_type = FACE;
}

void Face::setHE(HEdge* e) {
	_e = e;
}

HEdge* Face::getHE() {
	return _e;
}

bool Face::hit(Ray& ray, double t0, double t1, hitRecord& rec) {
	HEdge* edge = _e;
	Vertex* vert = edge->getVertex();
	Vector3d _p1 = vert->_p; Vector3d _n1 = vert->_n.normalized();
	edge = edge->getNext();
	vert = edge->getVertex();
	Vector3d _p2 = vert->_p; Vector3d _n2 = vert->_n.normalized();
	edge = edge->getNext();
	vert = edge->getVertex();
	Vector3d _p3 = vert->_p; Vector3d _n3 = vert->_n.normalized();

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
		if (smooth) {
			rec.n = ((1 - gamma - beta) * _n1 + beta * _n2 + gamma * _n3).normalized();
		} else {
			rec.n = _normal;
		}
		rec.m = _m;
	}
	return true;
}

void Face::boundingBox() {
	HEdge* edge = _e;
	Vertex* vert = edge->getVertex();
	Vector3d _p1 = vert->_p;
	edge = _e->getNext();
	vert = edge->getVertex();
	Vector3d _p2 = vert->_p;
	edge = edge->getNext();
	vert = edge->getVertex();
	Vector3d _p3 = vert->_p;
//	std::cout << "whhatnow" << std::endl;
//	std::cout << _p1 << std::endl << _p2 << std::endl << _p3 << std::endl;
	_b.MAX = _p1.cwiseMax(_p2.cwiseMax(_p3));
	_b.MIN = _p1.cwiseMin(_p2.cwiseMin(_p3));
}

Face::~Face() {
	// TODO Auto-generated destructor stub
}
