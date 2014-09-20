/*
 * Ray.h
 *
 *  Created on: Dec 19, 2011
 *      Author: kamiyo
 */

#ifndef RAY_H_
#define RAY_H_

#include "UtilDefines.h"


class Ray {
public:
	Ray();
	Ray(Vector3d e, Vector3d d, std::vector<double> r, std::vector<Vector3d> a, int t);
	void reSign();
	virtual ~Ray();
		__m256d eye;
		__m256d dir;
		__m256d inv;
		__m256d sign;
		int type;
		enum { VIEW, SHAD, DIFF, OCCL };
		std::vector<double> ref;
		std::vector<Vector3d> alpha;
		static unsigned long count;

};

std::ostream &operator<<(std::ostream &os, Ray &r);

#endif /* RAY_H_ */
