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
#include "Photon.h"
#include "Ray.h"
#include "BBox.h"

class Surface{
public:
	enum { NeedsToAlign = (sizeof(Vector4d) % 16) == 0 };
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF(NeedsToAlign);
	virtual bool _hit(RayBase& ray, const double t0, const double t1, hitRecord& rec) const;
	void trans(const Matrix4d& m, const Matrix4d& inv);
	virtual bool hit(RayBase& ray, const double t0, const double t1, hitRecord& rec) const = 0;
	virtual void boundingBox() = 0;
	virtual void renderBoundingBox(std::vector<std::vector<float> >& verts, int level) const;
	virtual int type();
	Matrix4d _mInv = Matrix4d::Identity();
	Matrix4d _mTrans = Matrix4d::Identity();
	s_ptr<Material> _m;
	bool _trans;
	BBox _b;
	int _type;
	enum { PLANE, SPHERE, CYLINDER, CONE, TORUS, TRIANGLE, FACE, CIRCLE, BOX, BOVOH, INSTANCE };
};

#endif /* SURFACE_H_ */
