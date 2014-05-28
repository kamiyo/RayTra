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
	if (beta < 0 || beta >(1 - gamma)) {
		return false;
	}
	if (ray.type == Ray::VIEW) {
		rec.t = t;
		if (smooth) {
			rec.n = ((1 - gamma - beta) * _n1 + beta * _n2 + gamma * _n3).normalized();
		}
		else {
			rec.n = _normal;
		}
	}
	else if (ray.type == Ray::SHAD) {
		rec.s = this;
	}
	rec.m = _m;
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
	_b.set(_p1.cwiseMin(_p2.cwiseMin(_p3)), _p1.cwiseMax(_p2.cwiseMax(_p3)));
}

Face::~Face() {
	// TODO Auto-generated destructor stub
}
