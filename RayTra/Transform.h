#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "UtilDefines.h"

class Transform
{
public:
	Transform();
	virtual ~Transform();
	void setIdentity();
	void push();
	void push(Matrix4d &m);
	Matrix4d pop();
	Matrix4d top();
	void scale(double s);
	void scale(Vector3d xyz);
	void translate(Vector3d xyz);
	void rotate(Vector3d axis, double rot);
	Matrix4d _current;
	Matrix4d _currentInv;
	std::stack<Matrix4d> _m;
	std::stack<Matrix4d> _mInv;
};

inline Vector4d operator* (const Transform& t, const Vector4d& v) {
	return t._currentInv * v;
}

inline Vector3d apply(const Matrix4d& m, const Vector3d& v, const int& i) {
	Vector4d vv(v(0), v(1), v(2), (double)i);
	return (m * vv).block<3, 1>(0, 0);
}
#endif