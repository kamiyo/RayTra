/*
 * Triangle.h
 *
 *  Created on: Dec 4, 2011
 *      Author: kamiyo
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "Surface.h"

class Triangle : public Surface {
public:
	Triangle(Vector3d p1, Vector3d p2, Vector3d p3, Material* m);
	bool hit(RayBase& ray, double t0, double t1, hitRecord& rec);
	void boundingBox();
	virtual ~Triangle();
	Vector3d _p1, _p2, _p3, abc, def, _n, _u1, _v1, _u2, _v2, _u3, _v3;

};

#endif /* TRIANGLE_H_ */
