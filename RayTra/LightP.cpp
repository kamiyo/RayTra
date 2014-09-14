/*
 * LightP.cpp
 *
 *  Created on: Dec 3, 2011
 *      Author: kamiyo
 */

#include "LightP.h"

LightP::LightP(Vector4d pos, Vector4d rgb, Vector4d atten, double r) {
	_pos = pos;
	_rgb = rgb;
	_r = r;
	_type = POINT;
	_atten = atten;
}

LightP::~LightP() {
	// TODO Auto-generated destructor stub
}

Vector4d LightP::getVector(Vector4d p) {
	return (_pos - p);
}

double LightP::getFalloff(Vector4d p) {
	double dist = (p - _pos).norm();
	return 1 / (_atten[0] + _atten[1] * dist + _atten[2] * dist * dist);
}
