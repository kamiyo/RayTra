/*
 * LightD.h
 *
 *  Created on: Dec 3, 2011
 *      Author: kamiyo
 */

#ifndef LIGHTD_H_
#define LIGHTD_H_
#include "Light.h"


class LightD : public Light {
public:
	LightD(Vector4d dir, Vector4d rgb, Vector4d atten);
	virtual ~LightD();
	virtual Vector4d getVector(Vector4d p);
	virtual double getFalloff(Vector4d p);
	Vector4d _dir;

};

#endif /* LIGHTD_H_ */
