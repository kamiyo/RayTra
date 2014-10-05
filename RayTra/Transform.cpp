#include "Transform.h"


Transform::Transform()
{
	setIdentity();
	push();
}


Transform::~Transform()
{
}

void Transform::setIdentity() {
	_current = _currentInv = _normal = Matrix4d::Identity();
}

void Transform::push() {
	_m.push(_current);
	_mInv.push(_currentInv);
}

void Transform::push(const Matrix4d &m) {
	_m.push(m);
	_mInv.push(m.inverse());
}

Matrix4d Transform::pop() {
	_current = _m.top();
	_currentInv = _mInv.top();
	_m.pop(); _mInv.pop();
	return _currentInv;
}

Matrix4d Transform::top() const {
	return _currentInv;
}

void Transform::scale(double s) {
	scale(Vector3d(s, s, s));
}

void Transform::scale(const Vector3d& xyz) {
	Matrix4d temp(Matrix4d::Identity()); temp.diagonal() << xyz(0), xyz(1), xyz(2), 1.0;
	Matrix4d tempInv(Matrix4d::Identity()); tempInv.diagonal() << 1 / xyz(0), 1 / xyz(1), 1 / xyz(2), 1.0;
	_current = temp * _current;
	_currentInv *= tempInv;
}

void Transform::translate(const Vector3d& xyz) {
	Matrix4d tempInv(Matrix4d::Identity()), temp(Matrix4d::Identity());
	temp.block<3, 1>(0, 3) = xyz;
	tempInv.block<3, 1>(0, 3) = -1 * xyz;
	_current = temp * _current;
	_currentInv *= tempInv;
}

void Transform::rotate(Vector3d axis, double rot) {
	rot = rot * M_PI / 180;
	axis.normalize();
	Matrix4d t(Matrix4d::Identity()), tInv(Matrix4d::Identity());
	t.block<3, 3>(0, 0) = AngleAxisd(rot, axis).toRotationMatrix();
	tInv.block<3, 3>(0, 0) = AngleAxisd(-rot, axis).toRotationMatrix();
	_current = t * _current;
	_currentInv *= tInv;
}

Matrix4d perspective(double right, double top, double near, double far) {
	Matrix4d m; m << near / right, 0, 0, 0,
		0, near / top, 0, 0,
		0, 0, -(far + near) / (far - near), -2 * far * near / (far - near),
		0, 0, -1, 0;
	return m;
}

Matrix4d lookat(const Vector3d& eye, const Vector3d& dir, const Vector3d& up) {
	Vector3d u = dir.normalized().cross(up.normalized()).normalized();
	Vector3d v = u.cross(dir.normalized());
	Vector3d w = -dir.normalized();
	Matrix4d m = Matrix4d::Identity();
	m << u(0), u(1), u(2), -u.dot(eye),
		v(0), v(1), v(2), -v.dot(eye),
		w(0), w(1), w(2), -w.dot(eye),
		    0,    0,    0,       1;
	return m;
}