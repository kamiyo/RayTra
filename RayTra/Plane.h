/*
 * Plane.h
 *
 *  Created on: Dec 4, 2011
 *      Author: kamiyo
 */

#ifndef PLANE_H_
#define PLANE_H_
#include "Surface.h"

class Plane : public Surface {
public:
	Plane(Vector3d n, Vector3d d, s_ptr<Material> m);
	virtual bool hit(RayBase& ray, double t0, double t1, hitRecord& rec) const;
	void renderBoundingBox(std::vector<std::vector<float> >& verts, std::vector<int> level) const;
	virtual void boundingBox();
	virtual ~Plane();
	Vector3d _p;
	Vector3d _n;
};

#endif /* PLANE_H_ */
