/*
 * Box.cpp
 *
 *  Created on: Dec 18, 2011
 *      Author: kamiyo
 */

#include "Box.h"
/*
VARS
vec		b[2]	(minX,minY,minZ) and (maxX,maxY,maxZ) of bounding box

*/

/*
Intialize to inf
*/
Box::Box() {
	MIN.setZero(); 
	MAX.setZero(); 
	MAX << nINF, nINF, nINF;
	MIN << INF, INF, INF;
	_m.setZero();
}

Box::Box(Vector3d min, Vector3d max) {
	MIN.setZero(); MAX.setZero();
	set(min, max);
}

Box::~Box() {
	// TODO Auto-generated destructor stub
}

Box Box::combine(Box& b1, Box& b2) {
	Box _b(b1.MIN.cwiseMin(b2.MIN), b1.MAX.cwiseMax(b2.MAX));
	return _b;
}

Box Box::combine(Box& rhs) {
	Box _b(MIN.cwiseMin(rhs.MIN), MAX.cwiseMax(rhs.MAX));
	return _b;
}

Box Box::transform(Matrix4d& m) {
	// if affine
	std::cout << m << std::endl;
	Box _b;
	/*if ((m.col(3).array() == Eigen::Array4d(0, 0, 0, 1)).all()) {
		std::cout << (*this) << std::endl;
		_b.set(m.block<1, 3>(3, 0), m.block<1, 3>(3, 0));
		Array33d A = m.block<3, 3>(0, 0).array().colwise() * MIN.array();
		Array33d B = m.block<3, 3>(0, 0).array().colwise() * MAX.array();
		Array33d min = A.cwiseMin(B);
		Array33d max = A.cwiseMax(B);
		_b.set(_b.MIN.array() + min.rowwise().sum(), _b.MAX.array() + max.rowwise().sum());
	}
	else {*/
		Eigen::Matrix<double, 8, 4> temp;
		temp << MIN[0], MIN[1], MIN[2], 1.0
			, MIN[0], MIN[1], MAX[2], 1.0
			, MIN[0], MAX[1], MIN[2], 1.0
			, MAX[0], MIN[1], MIN[2], 1.0
			, MIN[0], MAX[1], MAX[2], 1.0
			, MAX[0], MIN[1], MAX[2], 1.0
			, MAX[0], MAX[1], MIN[2], 1.0
			, MAX[0], MAX[1], MAX[2], 1.0;
		Eigen::Matrix<double, 4, 8> points = temp.transpose();
		std::cout << points << std::endl;
		Eigen::Matrix<double, 4, 8> result = m * points;
		std::cout << result << std::endl;
		Array4d min = result.rowwise().minCoeff();
		Array4d max = result.rowwise().maxCoeff();
		std::cout << min << " " << max << std::endl;
		_b.set(Vector3d(min(0), min(1), min(2)), Vector3d(max(0), max(1), max(2)));
	//}
	std::cout << _b << std::endl;
	return _b;
}

// Box::set also calculates midpoint
void Box::set(Vector3d min, Vector3d max) {
	MIN = min;
	MAX = max;
	_m = (MIN + MAX) / 2;
}

bool Box::operator==(const Box& rhs) {
	return ((MIN.array() == rhs.MIN.array()).all() && (MAX.array() == rhs.MAX.array()).all());
}
bool Box::operator!=(const Box& rhs) {
	return ((MIN.array() != rhs.MIN.array()).all() && (MAX.array() != rhs.MAX.array()).all());
}

std::ostream &operator<<(std::ostream &os, Box &b) {
	return os << "<" << b.MIN << "> <" << b.MAX << ">";
}