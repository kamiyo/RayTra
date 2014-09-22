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

Vector3d LightP::getVector(Vector3d p) {
	return (_pos - p);
}

double LightP::getFalloff(Vector3d p) {
	double dist = (p - _pos).norm();
	return 1 / (_atten[0] + _atten[1] * dist + _atten[2] * dist * dist);
}

// theta = 0 and phi = 0 gives point (0, 0, 1)
void LightP::projectScene(BBox b) {
	if ((_pos.array() < b.MAX.array()).all && (_pos.array() > b.MIN.array()).all()) {
		_sceneMap.set(Vector3d(0, 0, 1), Vector3d(2 * M_PI, M_PI, 1));
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
	temp = (temp.colwise() - _pos).colwise().normalized();
	Eigen::VectorXd theta(8);
	Eigen::VectorXd phi(8);
	for (int i = 0; i < 8; i++) {
		theta(i) = atan2(temp(1, i), temp(0, i));
		phi(i) = acos(temp(2, i));
	}
	_sceneMap.set(Vector3d(theta.minCoeff(), phi.minCoeff(), 1), Vector3d(theta.maxCoeff(), phi.maxCoeff(), 1));
}

Vector3d getRanPoint() {


}