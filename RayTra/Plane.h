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
	Plane(Vector4d n, Vector4d d, Material* m);
	virtual bool hit(Ray& ray, double t0, double t1, hitRecord& rec);
	virtual void boundingBox();
	virtual ~Plane();
	Vector4d _p;
	Vector4d _n;
};

#endif /* PLANE_H_ */
