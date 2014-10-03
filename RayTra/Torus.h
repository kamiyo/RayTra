#ifndef TORUS_H_
#define TORUS_H_
#include "Surface.h"
class Torus :
	public Surface
{
public:
	Torus(double R, double r, s_ptr<Material> m);
	void boundingBox();
	virtual bool hit(RayBase& ray, double t0, double t1, hitRecord& rec);
	~Torus();
	double _r, _R;
	double _R2, _R2r2;
	BBox o_b;
};

#endif /* TORUS_H_ */