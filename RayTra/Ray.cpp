/*
 * Ray.cpp
 *
 *  Created on: Dec 19, 2011
 *      Author: kamiyo
 */

#include "Ray.h"
/*
VARS
vec			eye		start
			dir		direction
			inv		direction^-1 (to prevent recalculation) see http://people.csail.mit.edu/amy/papers/box-jgt.pdf
vec			sign	for box-intersect algo
			type	type of ray (VIEW, SHAD, DIFF, OCCL)
vec-double	ref		vector of index of refractions (for nested transparent objects)
vec-vec		alpha	vector of rgb attenuation constants
*/



/*
IN
vec			e		start
			d		direction
vec-double	r		ref (see above)
vec-vec		a		alpha (see above)
int			t		type of ray
*/

unsigned long Ray::count = 0;

Ray::Ray(Vector4d e, Vector4d d, std::vector<double> r, std::vector<Vector4d> a, int t):
eye(e), dir(d), ref(r), alpha(a), type(t) {
#pragma omp critical
	{
		Ray::count = Ray::count + 1;
	}

	inv = dir.cwiseInverse();
	reSign();
	e(3) = 0;
	d(3) = 0;
	inv(3) = 0;
}

Ray::Ray() {
}

void Ray::reSign() {
	sign = (inv.array() < 0).cast<int>().block<3,1>(0, 0);
}

Ray::~Ray() {
	// TODO Auto-generated destructor stub
}

std::ostream &operator<<(std::ostream &os, Ray &r) {
	return os << r.eye << ", " << r.dir << " " << r.dir.norm() << " " << Ray::count;
}