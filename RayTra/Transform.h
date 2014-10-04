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
	void push(const Matrix4d &m);
	Matrix4d pop();
	Matrix4d top() const;
	void scale(double s);
	void scale(const Vector3d& xyz);
	void translate(const Vector3d& xyz);
	void rotate(Vector3d axis, double rot);
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

inline Vector3d apply(const Matrix4d& m, const Vector3d& v, const int& i) {
	double h = i;
	Vector4d vv(v(0), v(1), v(2), h);
	return (m * vv).block<3, 1>(0, 0);
}
#endif