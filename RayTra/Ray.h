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
		Vector3d eye;
		Vector3d dir;
		Vector3d inv;
		std::vector<int> sign;
		int type;
		static const int VIEW = 0, SHAD = 1, DIFF = 2, OCCL = 3;
		std::vector<double> ref;
		std::vector<Vector3d> alpha;
		static unsigned long count;

};

std::ostream &operator<<(std::ostream &os, Ray &r);

#endif /* RAY_H_ */
