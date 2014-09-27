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
	virtual Vector3d getRanPoint();
	virtual void projectScene(BBox b);
	Vector3d _pos;
	double _r, _costheta0;
	Vector3d _u, _v, _axis;
};

#endif /* LIGHTP_H_ */
