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
static const float flt_inf = -logf(0);
static const float flt_ninf = logf(0);

bool BVH::hitbox(Ray& ray, double t0, double t1) {
	//__m128 inf = _mm_load1_ps(&flt_inf);
	//__m128 ninf = _mm_load1_ps(&flt_ninf);


	double tmin, tmax, tymin, tymax, tzmin, tzmax;
	Vector4d e = ray.eye; e(3) = 0;
	Vector4d i = ray.inv; i(3) = 0;
	/*_b.MIN(3) = 0;
	_b.MAX(3) = 0;

	__m128 box_min = _mm_load_ps((float*)_b.MIN.data());
	__m128 box_max = _mm_load_ps((float*) _b.MAX.data());
	__m128 pos = _mm_load_ps((float*) e.data());
	__m128 inv = _mm_load_ps((float*) i.data());

	__m128 l1 = _mm_mul_ps(_mm_sub_ps(box_min, pos), inv);
	__m128 l2 = _mm_mul_ps(_mm_sub_ps(box_max, pos), inv);

	__m128 filtered_l1a = _mm_min_ps(l1, inf);
	__m128 filtered_l2a = _mm_min_ps(l2, inf);
	__m128 filtered_l1b = _mm_max_ps(l1, ninf);
	__m128 filtered_l2b = _mm_max_ps(l2, ninf);

	__m128 lmax = _mm_max_ps(filtered_l1a, filtered_l2a);
	__m128 lmin = _mm_min_ps(filtered_l1b, filtered_l2b);

	__m128 lmax0 = _mm_shuffle_ps(lmax, lmax, 0x39);
	__m128 lmin0 = _mm_shuffle_ps(lmin, lmin, 0x39);
	lmax = _mm_min_ss(lmax, lmax0);
	lmin = _mm_max_ss(lmin, lmin0);

	__m128 lmax1 = _mm_movehl_ps(lmax, lmax);
	__m128 lmin1 = _mm_movehl_ps(lmin, lmin);
	lmax = _mm_min_ss(lmax, lmax1);
	lmin = _mm_max_ss(lmin, lmin1);
	float ft0 = (float) t0;
	float ft1 = (float) t1;

	const bool ret = _mm_comige_ss(lmax, _mm_load1_ps(&ft0)) & _mm_comige_ss(lmax, lmin) & _mm_comige_ss(_mm_load1_ps(&ft1), lmin);
	float tt0, tt1;
	_mm_store_ss(&tt0, lmin);
	_mm_store_ss(&tt1, lmax);

	return ret;*/
	
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
