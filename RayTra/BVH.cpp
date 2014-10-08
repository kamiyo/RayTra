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

BVH::BVH(u_ptr<Group>& g) {
	_type = BOVOH;
	std::vector<u_ptr<Surface> >& s = g->_s;
	size_t N = s.size();
	if (N == 0) {
		_b = BBox();
		return;
	}
	if (N == 1) {
		_b = g->_b;
		_l = std::move(s[0]);
		_r = nullptr;
		return;
	}
	else if (N == 2) {
		_b = g->_b;
		_l = std::move(s[0]);
		_r = std::move(s[1]);
		return;
	}
	else {
		double m = 0;
		BBox b = g->_b;
		Vector3d diff = b.max() - b.min();
		int axis;
		double range = diff.maxCoeff(&axis);
		bool allSame = true;
		for (size_t i = 0; i < N; i++) {
			//std::cout << s[i]->_b << " " << s[i]->_b._m << std::endl;
			m += s[i]->_b._m[axis];
			if (allSame && (s[i]->_b != b && s[i]->_b._m[axis] != b._m[axis])) {
				allSame = false;
			}
		}
		m /= (double) N;
		//std::cout << m << " " << allSame << std::endl;
		u_ptr<Group> left = std::make_unique<Group>();
		u_ptr<Group> right = std::make_unique<Group>();
		for (size_t i = 0; i < N; i++) {
			if ((s[i]->_b._m[axis] < m && !allSame) || (allSame && i < N / 2)) {
				left->addSurface(s[i]);
			}
			else {
				right->addSurface(s[i]);
			}
		}
		_l = std::make_unique<BVH>(left);
		_r = std::make_unique<BVH>(right);
		_b = g->_b;
	}
}
BVH::~BVH() {
}

/*
traverses BVH and finds if ray hits any object
TODO: use this algo to render with OGL?
TODO: write tree display algo
POSSIBLE?: turn into Heap instead of tree
*/

// boxes should already be pre-transformed. BVH shouldn't have its own transformation
bool BVH::_hit(RayBase& ray, double t0, double t1, hitRecord& rec) const {
	/*if (!_b.hitbox(ray, t0, t1)) return false;
	if (_trans) {
		std::cout << "this BVH has been transformed" << std::endl;
		RayBase tRay(apply(_mInv, ray.m_eye, 1), apply(_mInv, ray.m_dir, 0), ray.m_type);
		bool temp = hit(tRay, t0, t1, rec);
		if (temp) {
			rec.n = apply(_mTrans, rec.n, 0);
			rec.n.normalize();
		}
		return temp;
	}
	else {*/
		return hit(ray, t0, t1, rec);
	//}
}

bool BVH::hit(RayBase& ray, double t0, double t1, hitRecord& rec) const {
	if (_b.hitbox(ray, t0, t1)) {
		hitRecord lrec, rrec;
		bool leftHit = (_l) && (_l->_hit(ray, t0, t1, lrec));
		bool rightHit = (_r) && (_r->_hit(ray, t0, t1, rrec));
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

void BVH::renderBoundingBox(std::vector<std::vector<float> >& verts, int level) const {
	if (level == 0) {
		_b.render(verts);
	}
	else {
		if (_l) _l->renderBoundingBox(verts, level - 1);
		if (_r) _r->renderBoundingBox(verts, level - 1);
	}
}


void BVH::boundingBox() {
}
