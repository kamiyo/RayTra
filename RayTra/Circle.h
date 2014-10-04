
#ifndef CIRCLE_H_
#define CIRCLE_H_
#include "Surface.h"
#include "Light.h"
#include <iostream>


class Circle : public Surface {
public:
	Circle(Vector3d pos, Vector3d norm, double rad, s_ptr<Material> m);
	Circle(Vector3d pos, Vector3d norm, double rad, s_ptr<Light> l);
	virtual bool hit(RayBase& ray, double t0, double t1, hitRecord& rec) const;
	void boundingBox();
	~Circle();
	Vector3d _p;
	Vector3d _n;
	double _r;
	s_ptr<Light> _l;
};

#endif