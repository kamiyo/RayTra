/*
 * Sphere.h
 *
 *  Created on: Dec 2, 2011
 *      Author: kamiyo
 */

#ifndef SPHERE_H_
#define SPHERE_H_
#include "Surface.h"
#include "LightP.h"
#include <iostream>

class Sphere : public Surface {
public:
	Sphere(Vector3d pos, double rad, Material* m);
	Sphere(LightP* l);
	virtual bool hit(Ray& ray, double t0, double t1, hitRecord& rec);
	void boundingBox();
	~Sphere();
	Vector3d _p;
	double _r;
	double _r2;
	LightP* _l;
};

#endif /* SPHERE_H_ */
