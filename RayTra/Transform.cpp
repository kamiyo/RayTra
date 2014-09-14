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

void Transform::push(Matrix4d &m) {
	_m.push(m);
	_mInv.push(m.inverse());
}

Matrix4d Transform::pop() {
	_current = _m.top();
	_currentInv = _mInv.top();
	_m.pop(); _mInv.pop();
	return _currentInv;
}

Matrix4d Transform::top() {
	return _currentInv;
}

void Transform::scale(double s) {
	scale(Vector4d(s, s, s, 1));
}

void Transform::scale(Vector4d xyz) {
	Matrix4d temp(Matrix4d::Identity()); temp.diagonal() << xyz(0), xyz(1), xyz(2), 1.0;
	Matrix4d tempInv(Matrix4d::Identity()); tempInv.diagonal() << 1 / xyz(0), 1 / xyz(1), 1 / xyz(2), 1.0;
	_current = temp * _current;
	_currentInv *= tempInv;
}

void Transform::translate(Vector4d xyz) {
	Matrix4d tempInv(Matrix4d::Identity()), temp(Matrix4d::Identity());
	temp.block<3, 1>(0, 3) = xyz.block<3, 1>(0, 0).eval();
	tempInv.block<3, 1>(0, 3) = -1 * xyz.block<3, 1>(0, 0).eval();
	_current = temp * _current;
	_currentInv *= tempInv;
}

void Transform::rotate(Vector4d axis, double rot) {
	rot = rot * M_PI / 180;
	axis.normalize();
	Matrix4d t(Matrix4d::Identity()), tInv(Matrix4d::Identity());
	t.block<3, 3>(0, 0) = AngleAxisd(rot, axis.block<3, 1>(0, 0).eval()).toRotationMatrix();
	tInv.block<3, 3>(0, 0) = AngleAxisd(-rot, axis.block<3, 1>(0, 0).eval()).toRotationMatrix();
	_current = t * _current;
	_currentInv *= tInv;
}
