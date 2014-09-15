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
#include "BBox.h"

class Surface {
public:
	virtual bool _hit(Ray& ray, double t0, double t1, hitRecord& rec);
	void trans(Matrix4d& m, Matrix4d& inv);
	virtual bool hit(Ray& ray, double t0, double t1, hitRecord& rec) = 0;
	virtual void boundingBox() = 0;
	virtual int type();
	Matrix4d _mInv;
	Matrix4d _mTrans;
	Material* _m;
	bool _trans;
	BBox _b;
	int _type;
	enum { PLANE, SPHERE, CYLINDER, CONE, TORUS, TRIANGLE, FACE, CIRCLE, BOX, BOVOH, INSTANCE };
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif /* SURFACE_H_ */
