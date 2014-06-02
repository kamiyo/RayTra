/*
 * Box.h
 *
 *  Created on: Dec 18, 2011
 *      Author: kamiyo
 */

#ifndef BOX_H_
#define BOX_H_

#include "UtilDefines.h"

#ifndef MAX
#define MAX b[1]
#endif

#ifndef MIN
#define MIN b[0]
#endif


class Box {
public:
	Box();
	Box(Vector3d min, Vector3d max);
	virtual ~Box();
	Box transform(Matrix4d& m);
	Box combine(Box& rhs);
	Box combine(Box& b1, Box& b2);
	Vector3d& max();
	Vector3d& min();
	void set(Vector3d min, Vector3d max);
	bool operator==(const Box& rhs);
	bool operator!=(const Box& rhs);
	Vector3d b[2];
	Vector3d _m;
};

std::ostream &operator<<(std::ostream &os, Box &b);

#endif /* BOX_H_ */
