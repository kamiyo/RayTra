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
	BVH(const u_ptr<Group>& g);
	bool hit(RayBase& ray, const double t0, const double t1, hitRecord& rec) const;
	bool _hit(RayBase& ray, const double t0, const double t1, hitRecord& rec) const;
	void renderBoundingBox(std::vector<std::vector<float> >& verts, int level) const;
	void boundingBox() {}
	u_ptr<Surface> _l;
	u_ptr<Surface> _r;
	virtual ~BVH();
};

#endif /* BVH_H_ */
