/*
 * Light.h
 *
 *  Created on: Dec 3, 2011
 *      Author: kamiyo
 */

#ifndef LIGHT_H_
#define LIGHT_H_
#include "UtilDefines.h"

class Light {
public:
	virtual ~Light();
	virtual Vector4d getVector(Vector4d p) = 0;
	virtual double getFalloff(Vector4d p) = 0;
	Vector4d _atten;
	Vector4d _rgb;
	int _type;
	enum {POINT, DIRECTIONAL, SPOT};
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif /* LIGHT_H_ */
