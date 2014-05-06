#include "Cylinder.h"


Cylinder::Cylinder(double r, double h, Material* m)
{
	_m = m;
	_r = r;
	_h = h;
	_type = CYLINDER;
	boundingBox();
}


Cylinder::~Cylinder()
{
}

bool Cylinder::hit(Ray& r, double t0, double t1, hitRecord& rec) {
	Vector3d eye = r.eye;
	Vector3d dir = r.dir;
	double a = dir[0] * dir[0] + dir[1] * dir[1];
	if (a == 0) return false;
	double b = 2 * (eye[0] * dir[0] + eye[1] * dir[1]);
	double c = eye[0] * eye[0] + eye[1] * eye[1] - _r * _r;
	double disc = b * b - 4 * a * c;
	if (disc < 0) return false;
	disc = sqrt(disc);
	rec.t = (-1.0 * b - disc) / (2 * a);
	if (rec.t > t1) return false;
	double z1 = eye[2] + rec.t * dir[2];
	if (rec.t < t0 || (z1 > (_h / 2.) || z1 < (_h / -2.))) {
		rec.t = (-1.0 * b + disc) / (2 * a);
		if (rec.t < t0 || rec.t > t1) return false;
		double z = eye[2] + rec.t * dir[2];
		if (z > _h / 2. || z < _h / -2.) {
			return false;
		}
		if (r.type == Ray::VIEW) {
			Vector3d n = eye + rec.t * dir;
			n[2] = 0;
			rec.n = -1.0 * n.normalized();
		}
		else if (r.type == Ray::SHAD) {
			rec.s = this;
		}
		rec.m = _m;
		return true;
	}
	else {
		rec.s = this;
		Vector3d n = eye + rec.t * dir;
		n[2] = 0;
		rec.n = n.normalized();
		rec.m = _m;
		return true;
	}


}

void Cylinder::boundingBox() {
	_b.MAX = Vector3d(_r, _r, _h/2.);
	_b.MIN = Vector3d(-_r, -_r, -_h/2.);
}