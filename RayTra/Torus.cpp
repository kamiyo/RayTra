#include "Torus.h"
#include "rpoly.cpp"


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
	Vector3d d = ray.dir;
	Vector3d e = ray.eye;
	double op[5];
	double dx = d(0), dy = d(1), dz = d(2);
	double dx2 = dx * dx, dy2 = dy * dy, dz2 = dz * dz;
	double ex = e(0), ey = e(1), ez = e(2);
	double ex2 = ex * ex, ey2 = ey * ey, ez2 = ez * ez;
	double dxdy = dx * dy, dxdz = dx * dz, dydz = dy * dz;
	double exey = ex * ey, exez = ex * ez, eyez = ey * ez;
	double dxex = dx * ex, dyey = dy * ey, dzez = dz * ez;
	double R2 = _R * _R, r2 = _r * _r;
	
	op[0] = pow2(dx2) + pow2(dy2) + pow2(dz2)
		  + 2 * (pow2(dxdy) + pow2(dxdz) + pow2(dydz));
	op[1] = 4 * ((dx2 + dy2 + dz2) * (dxex + dyey + dzez));
	op[2] = 2 * (-R2 * (dx2 + dy2 - dz2) - r2 * (dx2 + dy2 + dz2)
		  + 3 * (pow2(dxex) + pow2(dyey) + pow2(dzez))
		  + 4 * (dxdy * exey + dxdz * exez + dyey * dzez)
		  + dx2 * (ey2 + ez2) + dy2 * (ex2 + ez2) + dz2 * (ex2 + ey2));
	op[3] = 4 * (-R2 * (dxex + dyey - dzez) - r2 * (dxex + dyey + dzez)
		  + (ex2 + ey2 + ez2) * (dxex + dyey + dzez));
	op[4] = pow2(R2 - r2) + 2 * (-R2 * (ex2 + ey2 - ez2) - r2 * (ex2 + ey2 + ez2) + pow2(exey) + pow2(exez) + pow2(eyez))
		  + pow2(ex2) + pow2(ey2) + pow2(ez2);

	int Degree = 4;
	double zeroi[4], zeror[4];
	rpoly(op, &Degree, zeror, zeroi);
	std::vector<double> reals;
	for (int i = 0; i < 4; i++) {
		if (zeroi[i] == 0) {
			reals.push_back(zeror[i]);
		}
	}


}


Torus::~Torus()
{
}

void Torus::boundingBox() {
	double rR = _r + _R;
	_b.MAX = Vector3d(rR, _r, rR);
	_b.MIN = Vector3d(-rR, -_r, -rR);
}
