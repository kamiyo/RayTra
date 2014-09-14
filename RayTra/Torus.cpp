#include "Torus.h"
#include "rpoly.h"
#include <algorithm>


Torus::Torus(double R, double r, Material* m) : _r(r), _R(R)
{
	_m = m;
	_trans = false;
	boundingBox();
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
	const Vector4d d = ray.dir;
	const Vector4d e = ray.eye;
	double* op = (double*) malloc(sizeof(double) * 5);
	double dx = d(0), dy = d(1), dz = d(2);
	double dx2 = dx * dx, dy2 = dy * dy, dz2 = dz * dz;
	double ex = e(0), ey = e(1), ez = e(2);
	double ex2 = ex * ex, ey2 = ey * ey, ez2 = ez * ez;
	double dxdy = dx * dy, dxdz = dx * dz, dydz = dy * dz;
	double exey = ex * ey, exez = ex * ez, eyez = ey * ez;
	double dxex = dx * ex, dyey = dy * ey, dzez = dz * ez;
	double R2 = _R * _R, r2 = _r * _r;
	
	double A = d.dot(d);
	double B = 2 * d.dot(e);
	double C = e.dot(e) + (R2 - r2);
	double D = 4 * R2 * (dx2 + dy2);
	double E = 8 * R2 * (dxex + dyey);
	double F = 4 * R2 * (ex2 + ey2);
	/*
	op[0] = pow2(dx2) + pow2(dy2) + pow2(dz2)
	+ 2 * (pow2(dxdy) + pow2(dxdz) + pow2(dydz));
	op[1] = 4 * ((dx2 + dy2 + dz2) * (dxex + dyey + dzez));
	op[2] = 2 * (-R2 * (dx2 + dy2 - dz2) - r2 * (dx2 + dy2 + dz2)
	+ 3 * (pow2(dxex) + pow2(dyey) + pow2(dzez))
	+ 4 * (dxdy * exey + dxdz * exez + dyey * dzez)
	+ dx2 * (ey2 + ez2) + dy2 * (ex2 + ez2) + dz2 * (ex2 + ey2));
	op[3] = 4 * (R2 * (dxex + dyey + dzez) - r2 * (dxex + dyey + dzez)
	+ (ex2 + ey2 + ez2) * (dxex + dyey + dzez));
	op[4] = pow2(R2 - r2) + 2 * (R2 * (ex2 + ey2 + ez2) - r2 * (ex2 + ey2 + ez2) + pow2(exey) + pow2(exez) + pow2(eyez))
	+ pow2(ex2) + pow2(ey2) + pow2(ez2);
	*/
	op[0] = A * A;
	op[1] = 2 * A * B;
	op[2] = 2 * A * C + B * B - D;
	op[3] = 2 * B * C - E;
	op[4] = C * C - F;

	int Degree = 4;
	double* zeroi = (double*) malloc(sizeof(double) * 4), *zeror = (double*) malloc(sizeof(double) * 4);
	rpoly(op, &Degree, zeror, zeroi);
	std::vector<double> reals;
	for (int i = 0; i < Degree; i++) {
		if (zeroi[i] == 0 && zeror[i] >= t0 && zeror[i] < t1) {
			reals.push_back(zeror[i]);
		}
	}
	free(zeror); free(zeroi); free(op);
	zeror = zeroi = op = NULL;
	if ((size_t) reals.size() == 0) return false;
	std::sort(reals.begin(), reals.end());
	rec.t = reals[0];
	if (ray.type == Ray::VIEW) {
		Vector4d p = e + rec.t * d;
		Vector4d pp; pp << p(0), p(1), 0., 0.;
		Vector4d c = pp.normalized() * _R; // center of tube
		Vector4d n = (p - c).normalized();
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
	Vector4d i = ray.inv;
	Vector4d e = ray.eye;

	tmin = (_b.b[s[0]][0] - e[0]) * i[0];
	tmax = (_b.b[1 - s[0]][0] - e[0]) * i[0];
	tymin = (_b.b[s[1]][1] - e[1]) * i[1];
	tymax = (_b.b[1 - s[1]][1] - e[1]) * i[1];
	if ((tmin > tymax) || (tymin > tmax)) return false;
	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;
	tzmin = (_b.b[s[2]][2] - e[2]) * i[2];
	tzmax = (_b.b[1 - s[2]][2] - e[2]) * i[2];
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
	_b.set(Vector4d(-rR, -rR, -_r, 0), Vector4d(rR, rR, _r, 0));
}
