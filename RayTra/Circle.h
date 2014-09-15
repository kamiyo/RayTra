
#ifndef CIRCLE_H_
#define CIRCLE_H_
#include "Surface.h"
#include "Light.h"
#include <iostream>


class Circle : public Surface {
public:
	Circle(Vector4d pos, Vector4d norm, double rad, Material* m);
	Circle(Vector4d pos, Vector4d norm, double rad, Light* l);
	virtual bool hit(Ray& ray, double t0, double t1, hitRecord& rec);
	void boundingBox();
	~Circle();
	Vector4d _p;
	Vector4d _n;
	double _r;
	Light* _l;
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif