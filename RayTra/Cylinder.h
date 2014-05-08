#ifndef CYLINDER_H_
#define CYLINDER_H_

#include "Surface.h"
class Cylinder :
	public Surface
{
public:
	Cylinder(double r, double h, Material* m);
	~Cylinder();
	void boundingBox();
	virtual bool hit(Ray& ray, double t0, double t1, hitRecord& rec);
	double _r;
	double _h;
};
#endif