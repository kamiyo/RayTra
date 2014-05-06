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
	void addSurface(Surface* s);
	bool hit(Ray& ray, double t0, double t1, hitRecord& rec);
	void boundingBox();
	~Group();
	std::vector<Surface*> _s;
};

#endif /* GROUP_H_ */
