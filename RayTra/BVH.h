/*
 * BVH.h
 *
 *  Created on: Dec 17, 2011
 *      Author: kamiyo
 */

#ifndef BVH_H_
#define BVH_H_

#include "Group.h"
#include <vector>

class BVH : public Surface{
public:
	BVH(Group* g);
	bool hit(Ray& ray, double t0, double t1, hitRecord& rec);
	void boundingBox();
	bool hitbox(Ray& ray, double t0, double t1);
	Surface* _l;
	Surface* _r;
	virtual ~BVH();
};

#endif /* BVH_H_ */
