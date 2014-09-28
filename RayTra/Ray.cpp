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

Ray::Ray(Vector3d e, Vector3d d, std::vector<double> r, std::vector<Vector3d> a, int t):
RayBase(e, d, t), ref(r), alpha(a), type(t) {
//#pragma omp atomic
	Ray::count++;
}

Ray::Ray() {
}

Ray::~Ray() {
	// TODO Auto-generated destructor stub
}

std::ostream &operator<<(std::ostream &os, Ray &r) {
	return os << r.m_eye << ", " << r.m_dir << " " << r.m_dir.norm();
}