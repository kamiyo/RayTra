#ifndef INSTANCE_H_
#define INSTANCE_H_

#include "Surface.h"

class Instance : public Surface
{
public:
	Instance(Surface* s, Matrix4d& m);
	virtual bool hit(Ray& ray, double t0, double t1, hitRecord& rec);
	virtual void boundingBox();
	~Instance();
	Surface* _s;
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
#endif