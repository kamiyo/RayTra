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

class BBox {
public:
	BBox();
	BBox(Vector3d min, Vector3d max);
	virtual ~BBox();
	static BBox transform(const BBox& b, const Matrix4d& m);
	void transform(const Matrix4d& m);
	void combine(const BBox& rhs);
	static BBox combine(const BBox& b1, const BBox& b2);
	bool hitbox(RayBase& ray, const double t0, const double t1) const;
	void render(std::vector<std::vector<float> >& verts) const;
	Vector3d max() const { return _b[1]; }
	Vector3d min() const { return _b[0]; }
	Vector3d& max() { return _b[1]; }
	Vector3d& min() { return _b[0]; }
	double max(int i) const { return _b[1](i); }
	double min(int i) const { return _b[0](i); }
	void set(Vector3d min, Vector3d max);
	bool operator==(const BBox& rhs);
	bool operator!=(const BBox& rhs);
	Vector3d _b[2];
	Vector3d _m;
};

std::ostream &operator<<(std::ostream &os, BBox &b);

#endif /* BBOX_H_ */
