/*
 * Surface.h
 *
 *  Created on: Dec 2, 2011
 *      Author: kamiyo
 */

#ifndef SURFACE_H_
#define SURFACE_H_

#include "Transform.h"
#include "Material.h"
#include "Ray.h"
#include "Box.h"

class Surface {
public:
	enum { NeedsToAlign = (sizeof(Vector4d) % 16) == 0 };
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF(NeedsToAlign);
	bool _hit(Ray& ray, double t0, double t1, hitRecord& rec);
	void trans(Matrix4d& inv);
	virtual bool hit(Ray& ray, double t0, double t1, hitRecord& rec) = 0;
	virtual void boundingBox() = 0;
	virtual int type();
	virtual Box combine(const Box& b1, const Box& b2);
	Matrix4d _mInv;
	Matrix4d _mInvTrans;
	Material* _m;
	bool _trans;
	Box _b;
	int _type;
	enum { PLANE, SPHERE, CYLINDER, CONE, TORUS, TRIANGLE, FACE, CIRCLE, BOVOH, INSTANCE };
};

#endif /* SURFACE_H_ */
