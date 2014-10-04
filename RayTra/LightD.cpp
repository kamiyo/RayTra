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

Photon LightD::emitPhoton(int color) const {
	return Photon();
}

Vector3d LightD::getVector(Vector3d p) const {
	return _dir;
}

double LightD::getFalloff(Vector3d p) const {
	return 1.0;
}

// CHANGE CHANGE CHANGE
Vector3d LightD::getRanPoint() const {
	return Vector3d::Zero();
}

void LightD::projectScene(BBox b) {
	return;
}