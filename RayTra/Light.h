/*
 * Light.h
 *
 *  Created on: Dec 3, 2011
 *      Author: kamiyo
 */

#ifndef LIGHT_H_
#define LIGHT_H_
#include "UtilDefines.h"
#include "Photon.h"
#include "BBox.h"

class Light {
public:
	virtual ~Light();
	virtual Vector3d getVector(Vector3d p) = 0;
	virtual double getFalloff(Vector3d p) = 0;
	virtual Vector3d getRanPoint() = 0;
	virtual void projectScene(BBox b) = 0;
	Vector3d _atten;
	Vector3d _rgb;
	int _type;
	enum {POINT, DIRECTIONAL, SPOT};
};

#endif /* LIGHT_H_ */
