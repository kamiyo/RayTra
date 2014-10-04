#ifndef UNION_H_
#define UNION_H_
#include "Group.h"
class Union :
	public Group
{
public:
	Union();
	virtual bool hit(RayBase& ray, double t0, double t1, hitRecord& rec) const;
	~Union();
};

#endif