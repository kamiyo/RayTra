#include "Circle.h"


Circle::Circle(Vector3d pos, Vector3d norm, double rad, Material* m)
{
	_p = pos;
	_r = rad;
	_m = m;
	_n = norm;
	boundingBox();
	_l = NULL;
	_type = CIRCLE;
}

Circle::Circle(Vector3d pos, Vector3d norm, double rad, Light* l)
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

bool Circle::hit(Ray& ray, double t0, double t1, hitRecord& rec) {
	//std::cout << "here" << std::endl;
	Vector3d e = ray.eye;
	Vector3d d = ray.dir;
	//std::cout << "e = " << e << std::endl;
	//std::cout << "d = " << d << std::endl;
	double num = (_n.dot(_p - e));
	double den = _n.dot(d);
	if (den == 0) {
		return false;
	} else {
		rec.t = num / den;
		if (rec.t < t0 || rec.t > t1) {
			return false;
		}
		if (((e + d * rec.t) - _p).norm() > _r) {
			return false;
		}
		if (ray.type != Ray::OCCL) {
			if (ray.type != Ray::SHAD) {
				if (d.dot(_n) > 0) {
					rec.n = -1.0 * _n.normalized();
				} else {
					rec.n = _n.normalized();
				}
				rec.m = _m;
			} else {
				rec.s = this;
			}
		}
		return true;
	}
}

void Circle::boundingBox() {
	_b.MAX = _p.array() + _r;
	_b.MIN = _p.array() - _r;
}