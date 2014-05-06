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
