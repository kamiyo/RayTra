/*
 * LightD.cpp
 *
 *  Created on: Dec 3, 2011
 *      Author: kamiyo
 */

#include "LightD.h"

LightD::LightD(Vector4d dir, Vector4d rgb, Vector4d atten) {
	// TODO Auto-generated constructor stub
	_dir = dir;
	_rgb = rgb;
	_type = DIRECTIONAL;
	_atten = atten;
}

LightD::~LightD() {
	// TODO Auto-generated destructor stub
}

Vector4d LightD::getVector(Vector4d p) {
	return _dir;
}

double LightD::getFalloff(Vector4d p) {
	return 1.0;
}