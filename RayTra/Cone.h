#ifndef CONE_H_
#define CONE_H_

#include "Surface.h"
class Cone :
	public Surface
{
public:
	Cone(double h, Material* m);
	Cone(double lower, double upper, Material* m);
	void boundingBox();
	virtual bool hit(RayBase& ray, double t0, double t1, hitRecord& rec);
	~Cone();
	double _l, _u;
};

#endif /* CONE_H_ */