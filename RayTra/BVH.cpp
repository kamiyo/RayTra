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
	_type = BOVOH;
	std::vector<Surface*> s = g->_s;
	size_t N = s.size();
	if (N == 0) {
		return;
		_b = BBox();
	}
	if (N == 1) {
		_l = s[0];
		_r = NULL;
	}
	else if (N == 2) {
		_l = s[0];
		_r = s[1];
	}
	else {
		double m = 0;
		BBox b = g->_b;
		Vector4d diff = b.max() - b.min();
		int axis;
		double range = diff.maxCoeff(&axis);
		//std::cout << "----" << axis << "------" << std::endl;
		//std::cout << b << " " << b._m << std::endl;
		bool allSame = true;
		for (int i = 0; i < (int) N; i++) {
			//std::cout << s[i]->_b << " " << s[i]->_b._m << std::endl;
			m += s[i]->_b._m[axis];
			if (allSame && (s[i]->_b != b && s[i]->_b._m[axis] != b._m[axis])) {
				allSame = false;
			}
		}
		m /= (double) N;
		//std::cout << m << " " << allSame << std::endl;
		Group* left = new Group();
		Group* right = new Group();
		for (int i = 0; i < (int) N; i++) {
			if ((s[i]->_b._m[axis] < m && !allSame) || (allSame && i < (int) N / 2)) {
				left->addSurface(s[i]);
			}
			else {
				right->addSurface(s[i]);
			}
		}
		_l = new BVH(left);
		_r = new BVH(right);
	}
	_b = g->_b;
}
BVH::~BVH() {}

/*
traverses BVH and finds if ray hits any object
TODO: use this algo to render with OGL?
TODO: write tree display algo
POSSIBLE?: turn into Heap instead of tree
*/

bool BVH::_hit(Ray& ray, double t0, double t1, hitRecord& rec) {
	if (!hitbox(ray, t0, t1)) return false;
	if (_trans) {
		Ray tRay(apply(_mInv, ray.eye, 1), apply(_mInv, ray.dir, 0), ray.ref, ray.alpha, Ray::VIEW);
		Ray::count -= 1;
		bool temp = hit(tRay, t0, t1, rec);
		if (temp) {
			rec.n = apply(_mTrans, rec.n, 0);
			rec.n.normalize();
		}
		return temp;
	}
	else {
		return hit(ray, t0, t1, rec);
	}
}

bool BVH::hit(Ray& ray, double t0, double t1, hitRecord& rec) {
	if (hitbox(ray, t0, t1)) {
		hitRecord lrec, rrec;
		bool leftHit = (_l != NULL) && (_l->_hit(ray, t0, t1, lrec));
		bool rightHit = (_r != NULL) && (_r->_hit(ray, t0, t1, rrec));
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
	Vector4d e = ray.eye; e(3) = 0;
	Vector4d i = ray.inv; i(3) = 0;
	
	Vector3i s = ray.sign;

	tmin = (_b.b[s[0]][0] - e[0]) * i[0];
	tmax = (_b.b[1-s[0]][0] - e[0]) * i[0];
	tymin = (_b.b[s[1]][1] - e[1]) * i[1];
	tymax = (_b.b[1-s[1]][1] - e[1]) * i[1];
	if ((tmin > tymax) || (tymin > tmax)) return false;
	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;
	tzmin = (_b.b[s[2]][2] - e[2]) * i[2];
	tzmax = (_b.b[1-s[2]][2] - e[2]) * i[2];
	if ((tmin > tzmax) || (tzmin > tmax)) return false;
	if (tzmin > tmin) tmin = tzmin;
	if (tzmax < tmax) tmax = tzmax;
	return ((tmin < t1) && (tmax > t0));
}
