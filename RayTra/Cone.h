#ifndef CONE_H_
#define CONE_H_

#include "Surface.h"
class Cone :
	public Surface
{
public:
	Cone(double h, s_ptr<Material> m);
	Cone(double lower, double upper, s_ptr<Material> m);
	void boundingBox();
	virtual bool hit(RayBase& ray, double t0, double t1, hitRecord& rec) const;
	~Cone();
	double _l, _u;
};

#endif /* CONE_H_ */