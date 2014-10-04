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
	virtual Vector3d getVector(Vector3d p) const;
	virtual double getFalloff(Vector3d p) const;
	virtual Vector3d getRanPoint() const;
	virtual Photon emitPhoton(int color) const;
	virtual void projectScene(BBox b);
	Vector3d _dir;

};

#endif /* LIGHTD_H_ */
