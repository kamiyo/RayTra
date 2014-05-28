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

void Box::setMax(Vector3d max) {
	MAX = max;
}

void Box::setMin(Vector3d min) {
	MIN = min;
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