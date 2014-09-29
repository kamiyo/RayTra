/*
 * Plane.h
 *
 *  Created on: Dec 4, 2011
 *      Author: kamiyo
 */

#ifndef PLANE_H_
#define PLANE_H_
#include "Surface.h"

class Plane : public Surface {
public:
	Plane(Vector3d n, Vector3d d, Material* m);
	virtual bool hit(RayBase& ray, double t0, double t1, hitRecord& rec);
	virtual void boundingBox();
	virtual ~Plane();
	Vector3d _p;
	Vector3d _n;
};

#endif /* PLANE_H_ */
