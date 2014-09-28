
#ifndef CIRCLE_H_
#define CIRCLE_H_
#include "Surface.h"
#include "Light.h"
#include <iostream>


class Circle : public Surface {
public:
	Circle(Vector3d pos, Vector3d norm, double rad, Material* m);
	Circle(Vector3d pos, Vector3d norm, double rad, Light* l);
	virtual bool hit(RayBase& ray, double t0, double t1, hitRecord& rec);
	void boundingBox();
	~Circle();
	Vector3d _p;
	Vector3d _n;
	double _r;
	Light* _l;
};

#endif