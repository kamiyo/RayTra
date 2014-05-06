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
	LightD(Vector3d dir, Vector3d rgb, Vector3d atten);
	virtual ~LightD();
	virtual Vector3d getVector(Vector3d p);
	virtual double getFalloff(Vector3d p);
	Vector3d _dir;

};

#endif /* LIGHTD_H_ */
