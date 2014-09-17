#include "UtilDefines.h"

std::mt19937_64 mt;
std::uniform_real_distribution<double> uni_real;
std::uniform_int_distribution<int> uni_int;


Vector3d _store4d(const __m256d &d) {
	__declspec(align(32)) double res[4];
	_mm256_store_pd(res, d);
	return Vector3d(res[0], res[1], res[2]);
}

double _dot(const __m256d &a, const __m256d &b) {
	__m256d temp = _mm256_mul_pd(a, b);
	__declspec(align(32)) double res[4];
	__m256d mpte = _mm256_permute2f128_pd(temp, temp, 0x1);
	temp = _mm256_hadd_pd(temp, mpte);
	_mm256_store_pd(res, _mm256_hadd_pd(temp, temp));
	return res[0];
}

__m256d _load4d(const Vector3d &v) {
	const double* data = v.data();
	return _mm256_setr_pd(data[0], data[1], data[2], 0);
}

double _cross(const __m256d& a, const __m256d& b) { return 0; }

Vector3d randSphere() {
	double a, b, c;
	do {
		a = 2 * RAN - 1;
		b = 2 * RAN - 1;
		c = a*a + b*b;
	} while (c >= 1);
	Vector3d r;
	r.setZero();
	r << 2 * a * sqrt(1 - c), 2 * b * sqrt(1 - c), 1 - 2 * c;
	return r.normalized();
}

Vector3d cosVec(Vector3d a) {
	Vector3d r;
	double c;
	do {
		r = randSphere();
		r = r + a.normalized();
		c = r.norm();
	} while (c == 0);
	return r.normalized();
}

Vector3d _cosVec(Vector3d a) {
	Vector3d w = -1.0 * a;
	Vector3d up(0, 1, 0);
	if (a.dot(up) == 1) {
		up << 1, 0, 0;
	}
	Vector3d u = (up.cross(w)).normalized();
	Vector3d v = (w.cross(u)).normalized();
	double ep1 = 2 * M_PI * RAN;
	double ep2 = RAN;
	double ep21 = 1 - ep2;
	ep2 = sqrt(ep2);
	return Vector3d((cos(ep1) * ep2) * u + (sin(ep1) * ep2) * v - sqrt(ep21) * w).normalized();
}

void toDisk(double x, double y, Vector2d& v) {
	double theta = 2 * M_PI * x;
	double r = sqrt(y);
	v = Vector2d(r * cos(theta), r * sin(theta));
}
// Takes sample in 2-D square to circle
void to_unit_disk(double seedx, double seedy, Vector2d& v)
{
	double phi, r;
	double a = 2 * seedx - 1;   /* (a,b) is now on [-1,1]^2 */
	double b = 2 * seedy - 1;

	if (a > -b) {
		/* region 1 or 2 */
		if (a > b) {
			/* region 1, also |a| > |b| */
			r = a;
			phi = (M_PI / 4) * (b / a);
		}
		else       {
			/* region 2, also |b| > |a| */
			r = b;
			phi = (M_PI / 4) * (2 - (a / b));
		}
	}
	else {
		/* region 3 or 4 */
		if (a < b) {
			/* region 3, also |a| >= |b|, a != 0 */
			r = -a;
			phi = (M_PI / 4) * (4 + (b / a));
		}
		else       {
			/* region 4, |b| >= |a|, but a==0 and b==0 could occur. */
			r = -b;
			if (b != 0)
				phi = (M_PI / 4) * (6 - (a / b));
			else
				phi = 0;
		}
	}
	v << r * cos(phi), r * sin(phi);
}

void seedRand() {
	std::array<unsigned int, 2*std::mt19937_64::state_size> seed_data;
	std::random_device r;
	std::generate_n(seed_data.data(), seed_data.size(), std::ref(r));
	std::seed_seq seq(std::begin(seed_data), std::end(seed_data));

	mt = std::mt19937_64(seq);
	uni_real = std::uniform_real_distribution<double>(0., 1.);
}

double genRand_real() {
	return uni_real(mt);
}

int genRand_int(int x, int y) {
	std::uniform_int_distribution<int> ranint(x, y);
	return ranint(mt);
}

std::istream &operator>>(std::istream &is, Vector3d &f) {
	return is >> f[0] >> f[1] >> f[2];
}
std::ostream &operator<<(std::ostream &os, Vector3d &f) {
	return os << "<" << f[0] << "," << f[1] << "," << f[2] << ">";
}

Vector3d vmin(Vector3d &a, Vector3d &b) {
	return a.array().min(b.array());
}

Vector3d vmax(Vector3d &a, Vector3d &b) {
	return a.array().max(b.array());
}

Vector3d vmin(std::vector<Vector3d> &v) {
	Vector3d temp(INF, INF, INF);
	for (int i = 0; i < v.size(); i++) {
		temp = vmin(temp, v[i]);
	}
	return temp;
}

Vector3d vmax(std::vector<Vector3d> &v) {
	Vector3d temp(nINF, nINF, nINF);
	for (int i = 0; i < v.size(); i++) {
		temp = vmax(temp, v[i]);
	}
	return temp;
}

std::ostream &operator<<(std::ostream &os, Sampler2d &s) {
	for (int i = 0; i < s.size(); i++) {
		os << s[i][0] << " " << s[i][1] << std::endl;
		os << "--- " << s[i].norm() << " ---";
		if (i != s.size() - 1) {
			os << "\n";
		}
	}
	return os;
}