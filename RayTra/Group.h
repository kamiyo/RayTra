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
	void addSurface(s_ptr<Surface> s);
	virtual bool hit(RayBase& ray, double t0, double t1, hitRecord& rec);
	void boundingBox();
	~Group();
	std::vector<s_ptr<Surface> > _s;
};

#endif /* GROUP_H_ */
