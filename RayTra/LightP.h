/*
 * LightP.h
 *
 *  Created on: Dec 3, 2011
 *      Author: kamiyo
 */

#ifndef LIGHTP_H_
#define LIGHTP_H_
#include "Light.h"


class LightP : public Light {
public:
	LightP(Vector4d pos, Vector4d rgb, Vector4d atten, double r);
	virtual ~LightP();
	virtual Vector4d getVector(Vector4d p);
	virtual double getFalloff(Vector4d p);
	Vector4d _pos;
	double _r;
};

#endif /* LIGHTP_H_ */
