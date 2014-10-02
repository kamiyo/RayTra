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
	bool hit(RayBase& ray, double t0, double t1, hitRecord& rec);
	bool _hit(RayBase& ray, double t0, double t1, hitRecord& rec);
	void boundingBox();
	Surface* _l;
	Surface* _r;
	virtual ~BVH();
};

#endif /* BVH_H_ */
