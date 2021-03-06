#include "Torus.h"
#include <unsupported\Eigen\Polynomials>
#include <algorithm>


Torus::Torus(double R, double r, Material* m) : _r(r), _R(R)
{
	_m = m;
	_trans = false;
	boundingBox();
	_R2 = _R * _R;
	_R2r2 = _R2 - (_r * _r);
}

inline double pow2(const double& x) {
	return x * x;
}

inline double pow3(const double& x) {
	return pow2(x) * x;
}

inline double pow4(const double& x) {
	double p2 = pow2(x);
	return p2 * p2;
}


bool Torus::hit(Ray& ray, double t0, double t1, hitRecord& rec) {
	//if (!hitbox(ray, t0, t1)) return false;
	const Vector3d d = ray.dir;
	const Vector3d e = ray.eye;
	double dx2 = d(0) * d(0), dy2 = d(1) * d(1);
	double ex2 = e(0) * e(0), ey2 = e(1) * e(1);
	double dxex = d(0) * e(0), dyey = d(1) * e(1);
	
	double A = d.dot(d);
	double B = 2 * d.dot(e);
	double C = e.dot(e) + (_R2r2);
	double D = 4 * _R2 * (dx2 + dy2);
	double E = 8 * _R2 * (dxex + dyey);
	double F = 4 * _R2 * (ex2 + ey2);

	Vector5d op;
	op << C * C - F, 2 * B * C - E, 2 * A * C + B * B - D, 2 * A * B, A * A;

	Eigen::PolynomialSolver<double, 4> psolve(op);
	std::vector<double> reals;
	psolve.realRoots(reals);

	for (int i = (int) reals.size() - 1; i >= 0; i--) {
		if (reals[i] < t0 || reals[i] > t1) {
			reals.erase(reals.begin() + i);
		}
	}
	
	if ((size_t) reals.size() == 0) return false;

	std::sort(reals.begin(), reals.end());
	rec.t = reals[0];
	if (ray.type == Ray::VIEW) {
		Vector3d p = e + rec.t * d;
		Vector3d pp; pp << p(0), p(1), 0.;
		Vector3d c = pp.normalized() * _R; // center of tube
		Vector3d n = (p - c).normalized();
		rec.n = n;
	}
	else {
		rec.s = this;
	}
	rec.m = _m;
	return true;
}

bool Torus::hitbox(Ray& ray, double t0, double t1) {
	double tmin, tmax, tymin, tymax, tzmin, tzmax;
	Vector3i s = ray.sign;
	Vector3d i = ray.inv;
	Vector3d e = ray.eye;

	tmin = (o_b.b[s[0]][0] - e[0]) * i[0];
	tmax = (o_b.b[1 - s[0]][0] - e[0]) * i[0];
	tymin = (o_b.b[s[1]][1] - e[1]) * i[1];
	tymax = (o_b.b[1 - s[1]][1] - e[1]) * i[1];
	if ((tmin > tymax) || (tymin > tmax)) return false;
	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;
	tzmin = (o_b.b[s[2]][2] - e[2]) * i[2];
	tzmax = (o_b.b[1 - s[2]][2] - e[2]) * i[2];
	if ((tmin > tzmax) || (tzmin > tmax)) return false;
	if (tzmin > tmin) tmin = tzmin;
	if (tzmax < tmax) tmax = tzmax;
	return ((tmin < t1) && (tmax > t0));
}

Torus::~Torus()
{
}

void Torus::boundingBox() {
	double rR = _r + _R;
	_b.set(Vector3d(-rR, -rR, -_r), Vector3d(rR, rR, _r));
	o_b = _b;
}
