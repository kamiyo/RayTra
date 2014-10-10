#include "Cone.h"


Cone::Cone(double h, s_ptr<Material> m) : _l(0), _u(h)
{
	_m = m;
	_type = CONE;
	boundingBox();
	_trans = false;
}

Cone::Cone(double lower, double upper, s_ptr<Material> m) : _l(lower), _u(upper)
{
	_m = m;
	_type = CONE;
	boundingBox();
	_trans = false;
}

bool Cone::hit(RayBase& ray, double t0, double t1, hitRecord& rec) const
{
	const Vector3d& eye = ray.m_eye;
	const Vector3d& dir = ray.m_dir;
	double a = dir(0) * dir(0) + dir(1) * dir(1) - dir(2) * dir(2);
	if (a == 0) return false;
	double b = 2 * (eye(0) * dir(0) + eye(1) * dir(1) - eye(2) * dir(2));
	double c = eye(0) * eye(0) + eye(1) * eye(1) - eye(2) * eye(2);
	double disc = b * b - 4 * a * c;
	if (disc < 0) return false;
	disc = sqrt(disc);
	rec.t = (-1.0 * b - disc) / (2 * a);
	if (rec.t > t1) return false;
	double z1 = eye(2) + rec.t * dir(2);
	if ((rec.t < t0) || (z1 > _u) || (z1 < _l)) {
		rec.t = (-1.0 * b + disc) / (2 * a);
		if (rec.t < t0 || rec.t > t1) return false;
		double z = eye(2) + rec.t * dir(2);
		if (z > _u || z < _l) return false;
		if (ray.m_type != RayBase::SHADOW) {
			Vector3d n = eye + rec.t * dir;
			double nz; nz = (n(2) > 0) ? -1.0 : 1.0;
			n(2) = 0; n.normalize();
			n(2) = nz;
			rec.n = n.normalized();
		}
		rec.m = _m;
		return true;
	}
	else {
		if (ray.m_type != RayBase::SHADOW) {
			Vector3d n = eye + rec.t * dir;
			double nz; nz = (n(2) > 0) ? -1.0 : 1.0;
			n(2) = 0; n.normalize();
			n(2) = nz;
			rec.n = n.normalized();
			rec.m = _m;
		}
		return true;
	}
}

void Cone::boundingBox() {
	double maxRad = std::max(abs(_u), abs(_l));
	_b.set(Vector3d(-maxRad, -maxRad, _l), Vector3d(maxRad, maxRad, _u));
}

Cone::~Cone()
{
}
