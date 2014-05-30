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
	scale(Vector3d(s, s, s));
}

void Transform::scale(Vector3d xyz) {
	Matrix4d temp(Matrix4d::Identity()); temp.diagonal() << xyz(0), xyz(1), xyz(2), 1.0;
	std::cout << temp << std::endl;
	Matrix4d tempInv(Matrix4d::Identity()); tempInv.diagonal() << 1 / xyz(0), 1 / xyz(1), 1 / xyz(2), 1.0;
	std::cout << tempInv << std::endl;
	_current *= temp;
	_currentInv = tempInv * _currentInv;
}

void Transform::translate(Vector3d xyz) {
	Matrix4d tempInv(Matrix4d::Identity()), temp(Matrix4d::Identity());
	temp.block<3, 1>(0, 3) = xyz;
	tempInv.block<3, 1>(0, 3) = -1 * xyz;
	_current *= temp;
	_currentInv = tempInv * _currentInv;
}

void Transform::rotate(Vector3d axis, double rot) {
	rot = rot * M_PI / 180;
	std::cout << rot << std::endl;
	axis.normalize();
	Matrix4d t(Matrix4d::Identity()), tInv(Matrix4d::Identity());
	t.block<3, 3>(0, 0) = AngleAxisd(rot, axis).toRotationMatrix();
	tInv.block<3, 3>(0, 0) = AngleAxisd(-rot, axis).toRotationMatrix();
	_current *= t;
	_currentInv = tInv * _currentInv;
}
