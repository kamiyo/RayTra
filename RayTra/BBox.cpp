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
	min().setZero(); 
	max().setZero(); 
	max() << nINF, nINF, nINF;
	min() << INF, INF, INF;
	_m.setZero();
}

BBox::BBox(Vector3d in_min, Vector3d in_max) {
	min().setZero(); max().setZero();
	set(in_min, in_max);
}

BBox::~BBox() {
	// TODO Auto-generated destructor stub
}

/*
http://people.csail.mit.edu/amy/papers/box-jgt.pdf
gains of 25% in paper
*/
bool BBox::hitbox(RayBase& ray, const double t0, const double t1) const {
	double tmin, tmax, tymin, tymax, tzmin, tzmax;
	Vector3d e = ray.m_eye;
	Vector3d i = ray.m_inv;
	Vector3i s = ray.m_sign;

	tmin = (_b[s[0]][0] - e[0]) * i[0];
	tmax = (_b[1 - s[0]][0] - e[0]) * i[0];
	tymin = (_b[s[1]][1] - e[1]) * i[1];
	tymax = (_b[1 - s[1]][1] - e[1]) * i[1];
	if ((tmin > tymax) || (tymin > tmax)) return false;
	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;
	tzmin = (_b[s[2]][2] - e[2]) * i[2];
	tzmax = (_b[1 - s[2]][2] - e[2]) * i[2];
	if ((tmin > tzmax) || (tzmin > tmax)) return false;
	if (tzmin > tmin) tmin = tzmin;
	if (tzmax < tmax) tmax = tzmax;
	return ((tmin < t1) && (tmax > t0));
}

void BBox::render(std::vector<std::vector<float> >& verts) const {
	std::vector<float> temp;
	temp.push_back((float) min()(0));
	temp.push_back((float) min()(1));
	temp.push_back((float) min()(2));

	temp.push_back((float) min()(0));
	temp.push_back((float) min()(1));
	temp.push_back((float) max()(2));

	temp.push_back((float) min()(0));
	temp.push_back((float) max()(1));
	temp.push_back((float) max()(2));

	temp.push_back((float) min()(0));
	temp.push_back((float) max()(1));
	temp.push_back((float) min()(2));

	temp.push_back((float) min()(0));
	temp.push_back((float) min()(1));
	temp.push_back((float) min()(2));

	temp.push_back((float) max()(0));
	temp.push_back((float) min()(1));
	temp.push_back((float) min()(2));

	temp.push_back((float) max()(0));
	temp.push_back((float) max()(1));
	temp.push_back((float) min()(2));

	temp.push_back((float) min()(0));
	temp.push_back((float) max()(1));
	temp.push_back((float) min()(2));

	temp.push_back((float) min()(0));
	temp.push_back((float) max()(1));
	temp.push_back((float) max()(2));

	temp.push_back((float) max()(0));
	temp.push_back((float) max()(1));
	temp.push_back((float) max()(2));

	temp.push_back((float) max()(0));
	temp.push_back((float) max()(1));
	temp.push_back((float) min()(2));

	temp.push_back((float) max()(0));
	temp.push_back((float) max()(1));
	temp.push_back((float) max()(2));

	temp.push_back((float) max()(0));
	temp.push_back((float) min()(1));
	temp.push_back((float) max()(2));

	temp.push_back((float) min()(0));
	temp.push_back((float) min()(1));
	temp.push_back((float) max()(2));

	temp.push_back((float) max()(0));
	temp.push_back((float) min()(1));
	temp.push_back((float) max()(2));

	temp.push_back((float) max()(0));
	temp.push_back((float) min()(1));
	temp.push_back((float) min()(2));

	verts.push_back(temp);
}

BBox BBox::combine(const BBox& b1, const BBox& b2) {
	BBox temp_b(b1.min().cwiseMin(b2.min()), b1.max().cwiseMax(b2.max()));
	return temp_b;
}

void BBox::combine(const BBox& rhs) {
	set(min().cwiseMin(rhs.min()), max().cwiseMax(rhs.max()));
}

BBox BBox::transform(const BBox& b, const Matrix4d& m) {
	BBox temp_b;
	Eigen::MatrixXd temp(8, 4);
	temp << b.min()[0], b.min()[1], b.min()[2], 1.0
		, b.min()[0], b.min()[1], b.max()[2], 1.0
		, b.min()[0], b.max()[1], b.min()[2], 1.0
		, b.max()[0], b.min()[1], b.min()[2], 1.0
		, b.min()[0], b.max()[1], b.max()[2], 1.0
		, b.max()[0], b.min()[1], b.max()[2], 1.0
		, b.max()[0], b.max()[1], b.min()[2], 1.0
		, b.max()[0], b.max()[1], b.max()[2], 1.0;
	temp.transposeInPlace();
	temp = m * temp;
	Array4d min = temp.rowwise().minCoeff();
	Array4d max = temp.rowwise().maxCoeff();
	temp_b.set(Vector3d(min(0), min(1), min(2)), Vector3d(max(0), max(1), max(2)));

	return temp_b;
}

void BBox::transform(const Matrix4d& m) {
	Eigen::MatrixXd temp(8, 4);
	temp <<min()[0],min()[1],min()[2], 1.0
		,min()[0],min()[1],max()[2], 1.0
		,min()[0],max()[1],min()[2], 1.0
		,max()[0],min()[1],min()[2], 1.0
		,min()[0],max()[1],max()[2], 1.0
		,max()[0],min()[1],max()[2], 1.0
		,max()[0],max()[1],min()[2], 1.0
		,max()[0],max()[1],max()[2], 1.0;
	temp.transposeInPlace();
	temp = m * temp;
	Array4d min = temp.rowwise().minCoeff();
	Array4d max = temp.rowwise().maxCoeff();
    set(Vector3d(min(0), min(1), min(2)), Vector3d(max(0), max(1), max(2)));
}


// BBox::set also calculates midpoint
void BBox::set(Vector3d in_min, Vector3d in_max) {
	min() = in_min;
	max() = in_max;
	_m = (min() + max()) / 2.0;
}

bool BBox::operator==(const BBox& rhs) {
	return ((min().array() == rhs.min().array()).all() && (max().array() == rhs.max().array()).all());
}
bool BBox::operator!=(const BBox& rhs) {
	return ((min().array() != rhs.min().array()).all() && (max().array() != rhs.max().array()).all());
}

std::ostream &operator<<(std::ostream &os, BBox &b) {
	return os << "<" << b.min() << "> <" << b.max() << ">";
}