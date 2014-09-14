/*
 * BBox.h
 *
 *  Created on: Dec 18, 2011
 *      Author: kamiyo
 */

#ifndef BBOX_H_
#define BBOX_H_

#include "UtilDefines.h"

#ifndef MAX
#define MAX b[1]
#endif

#ifndef MIN
#define MIN b[0]
#endif


class BBox {
public:
	BBox();
	BBox(Vector4d min, Vector4d max);
	virtual ~BBox();
	BBox transform(Matrix4d& m);
	BBox combine(BBox& rhs);
	BBox combine(BBox& b1, BBox& b2);
	Vector4d& max();
	Vector4d& min();
	void set(Vector4d min, Vector4d max);
	bool operator==(const BBox& rhs);
	bool operator!=(const BBox& rhs);
	Vector4d b[2];
	Vector4d _m;
};

std::ostream &operator<<(std::ostream &os, BBox &b);

#endif /* BBOX_H_ */
