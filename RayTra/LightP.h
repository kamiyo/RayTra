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
	LightP(Vector3d pos, Vector3d rgb, Vector3d atten, double r);
	virtual ~LightP();
	virtual Vector3d getVector(Vector3d p);
	virtual double getFalloff(Vector3d p);
	Vector3d _pos;
	double _r;
};

#endif /* LIGHTP_H_ */
