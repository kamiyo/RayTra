#include "Torus.h"
#include "rpoly.h"
#include <algorithm>


Torus::Torus(double R, double r, s_ptr<Material> m) : _r(r), _R(R)
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


bool Torus::hit(RayBase& ray, double t0, double t1, hitRecord& rec) const {
	//if (!hitbox(ray, t0, t1)) return false;
	const Vector3d& d = ray.m_dir;
	const Vector3d& e = ray.m_eye;
	double* op = (double*) malloc(sizeof(double) * 5);
	double dx = d(0), dy = d(1), dz = d(2);
	double dx2 = d(0) * d(0), dy2 = d(1)* d(1);
	double ex2 = e(0) * e(0), ey2 = e(1) * e(1);
	double dxex = d(0) * e(0), dyey = d(1) * e(1);

	double A = d.dot(d);
	double B = 2 * d.dot(e);
	double C = e.dot(e) + (_R2r2);
	double D = 4 * _R2 * (dx2 + dy2);
	double E = 8 * _R2 * (dxex + dyey);
	double F = 4 * _R2 * (ex2 + ey2);

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
	if (ray.m_type != RayBase::SHADOW) {
		Vector3d p = e + rec.t * d;
		Vector3d pp; pp << p(0), p(1), 0.;
		Vector3d c = pp.normalized() * _R; // center of tube
		Vector3d n = (p - c).normalized();
		rec.n = n;
	}
	rec.m = _m;
	return true;
}

Torus::~Torus()
{
}

void Torus::boundingBox() {
	double rR = _r + _R;
	_b.set(Vector3d(-rR, -rR, -_r), Vector3d(rR, rR, _r));
}
