/*
 * BBox.h
 *
 *  Created on: Dec 18, 2011
 *      Author: kamiyo
 */

#ifndef BBOX_H_
#define BBOX_H_

#include "UtilDefines.h"
#include "RayBase.h"

#ifndef MAX
#define MAX b[1]
#endif

#ifndef MIN
#define MIN b[0]
#endif


class BBox {
public:
	BBox();
	BBox(Vector3d min, Vector3d max);
	virtual ~BBox();
	BBox transform(Matrix4d& m);
	BBox combine(BBox& rhs);
	BBox combine(BBox& b1, BBox& b2);
	bool hitbox(RayBase& ray, const double t0, const double t1);
	Vector3d max();
	Vector3d min();
	void set(Vector3d min, Vector3d max);
	bool operator==(const BBox& rhs);
	bool operator!=(const BBox& rhs);
	Vector3d b[2];
	Vector3d _m;
};

std::ostream &operator<<(std::ostream &os, BBox &b);

#endif /* BBOX_H_ */
