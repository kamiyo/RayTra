#ifndef INSTANCE_H_
#define INSTANCE_H_

#include "Surface.h"

class Instance : public Surface
{
public:
	Instance(s_ptr<Surface> s, Matrix4d& m);
	virtual bool hit(RayBase& ray, double t0, double t1, hitRecord& rec);
	virtual void boundingBox();
	~Instance();
	s_ptr<Surface> _s;
};
#endif