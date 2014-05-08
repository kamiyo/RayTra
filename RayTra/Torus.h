#ifndef TORUS_H_
#define TORUS_H_
#include "Surface.h"
class Torus :
	public Surface
{
public:
	Torus(double R, double r, Material* m);
	void boundingBox();
	virtual bool hit(Ray& ray, double t0, double t1, hitRecord& rec);
	~Torus();
	double _r, _R;
};

#endif /* TORUS_H_ */