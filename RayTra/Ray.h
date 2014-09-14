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
	Ray(Vector4d e, Vector4d d, std::vector<double> r, std::vector<Vector4d> a, int t);
	void reSign();
	virtual ~Ray();
		Vector4d eye;
		Vector4d dir;
		Vector4d inv;
		Vector3i sign;
		int type;
		static const int VIEW = 0, SHAD = 1, DIFF = 2, OCCL = 3;
		std::vector<double> ref;
		std::vector<Vector4d> alpha;
		static unsigned long count;

};

std::ostream &operator<<(std::ostream &os, Ray &r);

#endif /* RAY_H_ */
