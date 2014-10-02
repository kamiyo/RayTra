/*
 * BBox.cpp
 *
 *  Created on: Dec 18, 2011
 *      Author: kamiyo
 */

#include "BBox.h"
/*
VARS
vec		b[2]	(minX,minY,minZ) and (maxX,maxY,maxZ) of bounding box

*/

/*
Intialize to inf
*/
BBox::BBox() {
	MIN.setZero(); 
	MAX.setZero(); 
	MAX << nINF, nINF, nINF;
	MIN << INF, INF, INF;
	_m.setZero();
}

BBox::BBox(Vector3d min, Vector3d max) {
	MIN.setZero(); MAX.setZero();
	set(min, max);
}

BBox::~BBox() {
	// TODO Auto-generated destructor stub
}

/*
http://people.csail.mit.edu/amy/papers/box-jgt.pdf
gains of 25% in paper
*/
bool BBox::hitbox(RayBase& ray, const double t0, const double t1) {
	double tmin, tmax, tymin, tymax, tzmin, tzmax;
	Vector3d e = ray.m_eye;
	Vector3d i = ray.m_inv;
	Vector3i s = ray.m_sign;

	tmin = (b[s[0]][0] - e[0]) * i[0];
	tmax = (b[1 - s[0]][0] - e[0]) * i[0];
	tymin = (b[s[1]][1] - e[1]) * i[1];
	tymax = (b[1 - s[1]][1] - e[1]) * i[1];
	if ((tmin > tymax) || (tymin > tmax)) return false;
	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;
	tzmin = (b[s[2]][2] - e[2]) * i[2];
	tzmax = (b[1 - s[2]][2] - e[2]) * i[2];
	if ((tmin > tzmax) || (tzmin > tmax)) return false;
	if (tzmin > tmin) tmin = tzmin;
	if (tzmax < tmax) tmax = tzmax;
	return ((tmin < t1) && (tmax > t0));
}

Vector3d BBox::min() {
	return MIN;
}

Vector3d BBox::max() {
	return MAX;
}

BBox BBox::combine(BBox& b1, BBox& b2) {
	BBox _b(b1.MIN.cwiseMin(b2.MIN), b1.MAX.cwiseMax(b2.MAX));
	return _b;
}

BBox BBox::combine(BBox& rhs) {
	BBox _b(MIN.cwiseMin(rhs.MIN), MAX.cwiseMax(rhs.MAX));
	return _b;
}

BBox BBox::transform(Matrix4d& m) {
	BBox _b;
	Eigen::MatrixXd temp(8, 4);
	temp << MIN[0], MIN[1], MIN[2], 1.0
		, MIN[0], MIN[1], MAX[2], 1.0
		, MIN[0], MAX[1], MIN[2], 1.0
		, MAX[0], MIN[1], MIN[2], 1.0
		, MIN[0], MAX[1], MAX[2], 1.0
		, MAX[0], MIN[1], MAX[2], 1.0
		, MAX[0], MAX[1], MIN[2], 1.0
		, MAX[0], MAX[1], MAX[2], 1.0;
	temp.transposeInPlace();
	temp = m * temp;
	Array4d min = temp.rowwise().minCoeff();
	Array4d max = temp.rowwise().maxCoeff();
	_b.set(Vector3d(min(0), min(1), min(2)), Vector3d(max(0), max(1), max(2)));

	return _b;
}

// BBox::set also calculates midpoint
void BBox::set(Vector3d min, Vector3d max) {
	MIN = min;
	MAX = max;
	_m = (MIN + MAX) / 2;
}

bool BBox::operator==(const BBox& rhs) {
	return ((MIN.array() == rhs.MIN.array()).all() && (MAX.array() == rhs.MAX.array()).all());
}
bool BBox::operator!=(const BBox& rhs) {
	return ((MIN.array() != rhs.MIN.array()).all() && (MAX.array() != rhs.MAX.array()).all());
}

std::ostream &operator<<(std::ostream &os, BBox &b) {
	return os << "<" << b.MIN << "> <" << b.MAX << ">";
}