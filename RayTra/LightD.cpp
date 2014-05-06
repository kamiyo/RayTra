/*
 * LightD.cpp
 *
 *  Created on: Dec 3, 2011
 *      Author: kamiyo
 */

#include "LightD.h"

LightD::LightD(Vector3d dir, Vector3d rgb, Vector3d atten) {
	// TODO Auto-generated constructor stub
	_dir = dir;
	_rgb = rgb;
	_type = DIRECTIONAL;
	_atten = atten;
}

LightD::~LightD() {
	// TODO Auto-generated destructor stub
}

Vector3d LightD::getVector(Vector3d p) {
	return _dir;
}

double LightD::getFalloff(Vector3d p) {
	return 1.0;
}