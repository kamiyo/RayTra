/*
* BVH.cpp
*
*  Created on: Dec 17, 2011
*      Author: kamiyo
*/

#include "BVH.h"
/*
Bounding Volume Hierarchy (kd-tree)
INHERITED from SURFACE
material*	_m
box			_b
int			_type
VARS
surface*	_l	pointer to left node
surface*	_r	pointer to right node
*/

BVH::BVH(Group* g) {
	BVH(g, 0);
	_type = BOVOH;
}

/* splits Group g into subtrees l and r based on axis

TODO: do we need axis as argument?

*/
BVH::BVH(Group* g, int axis) {
	std::vector<Surface*> s = g->_s;
	int N = (int) s.size();
	if (N == 0) {
		return;
	}
	if (N == 1) {
		_l = s[0];
		_r = NULL;
		_b = _l->_b;
	}
	else if (N == 2) {
		_l = s[0];
		_r = s[1];
		_b = Surface::combine(_l->_b, _r->_b);
	}
	else {
		double m; double range = 0;
		for (int i = 0; i < 3; i++) {
			double tRange = _b.MAX[i] - _b.MIN[i];
			if (tRange > range) {
				axis = i;
				range = tRange;
			}
		}
		Box b = g->_b;
		double result = 0;
		bool allSame = true;
		double prev;
		for (int i = 0; i < (int) s.size(); i++) {
			double temp = (s[i]->_b.MAX[axis] + s[i]->_b.MIN[axis]) / 2;
			result += temp;
			if (i == 0) {
				prev = temp;
			}
			else if (allSame == true){
				if (prev != temp)
					allSame = false;
			}
		}
		result /= (int) s.size();
		m = (b.MAX[axis] + b.MIN[axis]) / 2;
		Group* left = new Group();
		Group* right = new Group();
		for (int i = 0; i < (int) s.size(); i++) {
			if (((s[i]->_b.MAX[axis] + s[i]->_b.MIN[axis]) / 2 < result && !allSame) || (allSame && i < (int) s.size() / 2)) {
				left->addSurface(s[i]);
			}
			else {
				right->addSurface(s[i]);
			}
		}
		_l = new BVH(left, (axis + 1) % 3);
		_r = new BVH(right, (axis + 1) % 3);
		_b = Surface::combine(_l->_b, _r->_b);
	}
}
BVH::~BVH() {}

/*
traverses BVH and finds if ray hits any object
TODO: use this algo to render with OGL?
TODO: right tree display algo
POSSIBLE?: turn into Heap instead of tree
*/

bool BVH::hit(Ray& ray, double t0, double t1, hitRecord& rec) {
	if (hitbox(ray, t0, t1)) {
		hitRecord lrec, rrec;
		bool leftHit = (_l != NULL) && (_l->hit(ray, t0, t1, lrec));
		bool rightHit = (_r != NULL) && (_r->hit(ray, t0, t1, rrec));
		if (leftHit && rightHit) {
			if (lrec.t < rrec.t) {
				rec = lrec;
			}
			else {
				rec = rrec;
			}
			return true;
		}
		else if (leftHit) {
			rec = lrec;
			return true;
		}
		else if (rightHit) {
			rec = rrec;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

void BVH::boundingBox() {
}

/*
http://people.csail.mit.edu/amy/papers/box-jgt.pdf
gains of 25% in paper
*/
bool BVH::hitbox(Ray& ray, double t0, double t1) {
	double tmin, tmax, tymin, tymax, tzmin, tzmax;
	
	tmin = (_b.b[ray.sign[0]][0] - ray.eye[0]) * ray.inv[0];
	tmax = (_b.b[1-ray.sign[0]][0] - ray.eye[0]) * ray.inv[0];
	tymin = (_b.b[ray.sign[1]][1] - ray.eye[1]) * ray.inv[1];
	tymax = (_b.b[1-ray.sign[1]][1] - ray.eye[1]) * ray.inv[1];
	if ((tmin > tymax) || (tymin > tmax)) return false;
	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;
	tzmin = (_b.b[ray.sign[2]][2] - ray.eye[2]) * ray.inv[2];
	tzmax = (_b.b[1-ray.sign[2]][2] - ray.eye[2]) * ray.inv[2];
	if ((tmin > tzmax) || (tzmin > tmax)) return false;
	if (tzmin > tmin) tmin = tzmin;
	if (tzmax < tmax) tmax = tzmax;
	return ((tmin < 1) && (tmax > 0));
}
