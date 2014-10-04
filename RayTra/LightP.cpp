/*
 * LightP.cpp
 *
 *  Created on: Dec 3, 2011
 *      Author: kamiyo
 */

#include "LightP.h"

LightP::LightP(Vector3d pos, Vector3d rgb, Vector3d atten, double r) {
	_pos = pos;
	_rgb = rgb;
	_r = r;
	_type = POINT;
	_atten = atten;
}

LightP::~LightP() {
	// TODO Auto-generated destructor stub
}

Vector3d LightP::getVector(Vector3d p) const {
	return (_pos - p);
}

double LightP::getFalloff(Vector3d p) const {
	double dist = (p - _pos).norm();
	return 1 / (_atten[0] + _atten[1] * dist + _atten[2] * dist * dist);
}

void LightP::projectScene(BBox b) {
	if ((_pos.array() < b.MAX.array()).all() && (_pos.array() > b.MIN.array()).all()) {
		_axis = Vector3d(nINF, nINF, nINF);
		return;
	}
	Eigen::MatrixXd temp(8, 3);
	temp << b.MIN[0], b.MIN[1], b.MIN[2]
		, b.MIN[0], b.MIN[1], b.MAX[2]
		, b.MIN[0], b.MAX[1], b.MIN[2]
		, b.MAX[0], b.MIN[1], b.MIN[2]
		, b.MIN[0], b.MAX[1], b.MAX[2]
		, b.MAX[0], b.MIN[1], b.MAX[2]
		, b.MAX[0], b.MAX[1], b.MIN[2]
		, b.MAX[0], b.MAX[1], b.MAX[2];
	temp.transposeInPlace();
	std::cout << temp << std::endl;
	temp = (temp.colwise() - _pos).colwise().normalized();
	std::cout << temp << std::endl;
	Vector3d v0, v1; double largestSquared = 0;
	for (int i = 0; i < (temp.size() / 3); i++) {
		Vector3d tv0 = temp.col(i);
		for (int j = i + 1; j < (temp.size() / 3); j++) {
			Vector3d tv1 = temp.col(j);
			double tempNorm = (tv0 - tv1).squaredNorm();
			if (tempNorm > largestSquared) {
				largestSquared = tempNorm;
				v0 = tv0;
				v1 = tv1;
			}
		}
	}
	_axis = (v0 + v1).normalized();
	_costheta0 = v0.dot(_axis);
	Vector3d w = -_axis;
	Vector3d up = (w.dot(Vector3d(0, 1, 0)) == 1) ? Vector3d(1, 0, 0) : Vector3d(0, 1, 0);
	_u = up.cross(w).normalized();
	_v = w.cross(_u).normalized();
}

Vector3d LightP::getRanPoint() const {
	if ((_axis.cwiseEqual(nINF)).all()) {
		return randSphere();
	}
	double costheta = (1 - _costheta0) * RAN + _costheta0;
	double phi = M_PI * (2 * RAN - 1);
	double sintheta = sqrt(1 - costheta * costheta);
	Vector3d result = sintheta * (cos(phi) * _u + sin(phi) * _v) + costheta * _axis;
	return result + _pos;
}

Photon LightP::emitPhoton(int color) const {
	Vector3d point = getRanPoint();
	Vector3d dir = COSVEC(-getVector(point));
	return Photon(point, dir, _rgb[color], color);
}