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

class Sphere : public Surface {
public:
	Sphere(Vector3d pos, double rad, s_ptr<Material> m);
	Sphere(s_ptr<LightP> l);
	virtual bool hit(RayBase& ray, double t0, double t1, hitRecord& rec) const;
	void boundingBox();
	~Sphere();
	Vector3d _p;
	double _r;
	double _r2;
	s_ptr<LightP> _l;
};

#endif /* SPHERE_H_ */
