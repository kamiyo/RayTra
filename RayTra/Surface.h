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
#include "Photon.h"
#include "BBox.h"

class Surface {
public:
	enum { NeedsToAlign = (sizeof(Vector4d) % 16) == 0 };
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF(NeedsToAlign);
	virtual bool _hit(Ray& ray, double t0, double t1, hitRecord& rec);
	virtual bool _hit(Photon& photon, double t0, double t1, hitRecord& rec);
	void trans(Matrix4d& m, Matrix4d& inv);
	virtual bool hit(Ray& ray, double t0, double t1, hitRecord& rec) = 0;
	virtual bool hit(Photon& photon, double t0, double t1, hitRecord& rec) = 0;
	virtual void boundingBox() = 0;
	virtual int type();
	Matrix4d _mInv;
	Matrix4d _mTrans;
	Material* _m;
	bool _trans;
	BBox _b;
	int _type;
	enum { PLANE, SPHERE, CYLINDER, CONE, TORUS, TRIANGLE, FACE, CIRCLE, BOX, BOVOH, INSTANCE };
};

#endif /* SURFACE_H_ */
