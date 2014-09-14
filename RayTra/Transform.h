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
	void scale(Vector4d xyz);
	void translate(Vector4d xyz);
	void rotate(Vector4d axis, double rot);
	Matrix4d _current;
	Matrix4d _currentInv;
	Matrix4d _normal;
	std::stack<Matrix4d> _m;
	std::stack<Matrix4d> _mInv;
	std::stack<Matrix4d> _mN;
};

inline Vector4d operator* (const Transform& t, const Vector4d& v) {
	return t._currentInv * v;
}

inline Vector4d apply(const Matrix4d& m, const Vector4d& v, const int& i) {
	double h = i;
	Vector4d vv(v(0), v(1), v(2), h);
	vv = m * vv; vv(3) = 0;
	return vv;
}
#endif