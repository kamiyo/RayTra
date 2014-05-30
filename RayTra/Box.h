/*
 * Box.h
 *
 *  Created on: Dec 18, 2011
 *      Author: kamiyo
 */

#ifndef BOX_H_
#define BOX_H_

#include "UtilDefines.h"
class Box {
public:
	Box();
	Box(Vector3d min, Vector3d max);
	virtual ~Box();
	Box transform(Matrix4d& m);
	Box combine(Box& rhs);
	Box combine(Box& b1, Box& b2);
	void set(Vector3d min, Vector3d max);
	bool operator==(const Box& rhs);
	bool operator!=(const Box& rhs);
	Vector3d b[2];
	Vector3d _m;
};

std::ostream &operator<<(std::ostream &os, Box &b);

#endif /* BOX_H_ */
