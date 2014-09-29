#include "Union.h"


Union::Union()
{
	BBox b;
	_b = b;
}

bool Union::hit(RayBase& ray, double t0, double t1, hitRecord& rec) {
	bool hit = false;
	for (int i = 0; i < (int) _s.size(); i++) {
		hitRecord temp;
		if (_s[i]->_hit(ray, t0, t1, temp)) {
			hit = true;
			rec = temp;
			t1 = rec.t;
		}
	}
	if (hit) {
		if (rec.t < t0 || rec.t > t1) return false;
	}
	return hit;
}

Union::~Union()
{
}
