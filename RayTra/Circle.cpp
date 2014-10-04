#include "Circle.h"


Circle::Circle(Vector3d pos, Vector3d norm, double rad, s_ptr<Material> m)
{
	_p = pos;
	_r = rad;
	_m = m;
	_n = norm;
	boundingBox();
	_l = NULL;
	_type = CIRCLE;
	_trans = false;
}

Circle::Circle(Vector3d pos, Vector3d norm, double rad, s_ptr<Light> l)
{
	_p = pos;
	_r = rad;
	_l = l;
	_n = norm;
	boundingBox();
	_m = NULL;
	_type = CIRCLE;
}



Circle::~Circle(void)
{
}

bool Circle::hit(RayBase& ray, double t0, double t1, hitRecord& rec) const {
	//std::cout << "here" << std::endl;
	const Vector3d& e = ray.m_eye;
	const Vector3d& d = ray.m_dir;
	double den = _n.dot(d);
	if (den == 0) {
		return false;
	}
	else {
		double num = (_n.dot(_p - e));
		rec.t = num / den;
		if (rec.t < t0 || rec.t > t1) {
			return false;
		}
		if (((e + d * rec.t) - _p).norm() > _r) {
			return false;
		}
		if (ray.m_type == RayBase::VIEW) {
			rec.n = _n.normalized();
		}
		else {
			;
		}
		rec.m = _m;

		return true;
	}
}

void Circle::boundingBox() {
	_b.set(_p.array() - _r, _p.array() + _r);
}