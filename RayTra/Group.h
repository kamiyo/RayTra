/*
 * Group.h
 *
 *  Created on: Dec 2, 2011
 *      Author: kamiyo
 */

#ifndef GROUP_H_
#define GROUP_H_

#include "Surface.h"

class Group : public Surface {
public:
	Group();
	void addSurface(u_ptr<Surface> &s);
	virtual bool hit(RayBase& ray, double t0, double t1, hitRecord& rec) const;
	void renderBoundingBox(std::vector<std::vector<float> >& verts, int level) const;
	void boundingBox();
	~Group();
	std::vector<u_ptr<Surface> > _s;
};

#endif /* GROUP_H_ */
