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
	void setMax(Vector3d max);
	void setMin(Vector3d min);
	void set(Vector3d min, Vector3d max);
	bool operator==(const Box& rhs);
	bool operator!=(const Box& rhs);
	Vector3d b[2];
	Vector3d _m;
};

std::ostream &operator<<(std::ostream &os, Box &b);

#endif /* BOX_H_ */
