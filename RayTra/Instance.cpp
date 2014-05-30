#include "Instance.h"


Instance::Instance(Surface* s, Matrix4d& m) : _s(s)
{
	_b = _s->_b;
	_mInv = m.inverse();
	_mTrans = m.transpose();
	_type = INSTANCE;
}


Instance::~Instance()
{
}

bool Instance::hit(Ray& ray, double t0, double t1, hitRecord& rec) {
	Ray transRay(ray);
	Vector4d eye; eye.setZero(); eye << transRay.eye[0], transRay.eye[1], transRay.eye[2], 1;
	Vector4d dir; dir.setZero(); dir << transRay.dir[0], transRay.dir[1], transRay.dir[2], 0;
	eye = _mInv * eye;
	dir = _mInv * dir;
	transRay.eye << eye[0], eye[1], eye[2];
	transRay.dir << dir[0], dir[1], dir[2];
	if (_s->hit(transRay, t0, t1, rec)) {
		Vector4d n; n.setZero(); n << rec.n[0], rec.n[1], rec.n[2], 0;
		n = _mTrans * n;
		rec.n << n[0], n[1], n[2];
		return true;
	}
	return false;
}

void Instance::boundingBox() {


}