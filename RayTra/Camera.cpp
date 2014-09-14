/*
 * Camera.cpp
 *
 *  Created on: Dec 2, 2011
 *      Author: kamiyo
 */

#include "Camera.h"
/*
VARS
vec		_e		eye pos
_u		uvw coords (w points out from screen for right-handedness)
_v
_w
_dir	vec from plane to eye (= w if no shift)
double	_d		distance (focal length)
_width	image width
_height	image height
_size	lens size
int		_nx		width (pixels)
_ny		height (pixels)

*/

/*
IN
vec		ep		eye position
ip		image plane location
dir		image plane normal
fp		focal point
fd		focal direction
up		up-vector
double	d		distance (to image)
iw		width (units)
ih		height (units)
fstop	fstop (distance / size)
int		pw		width (pixels)
ph		height (pixels)
*/
Camera::Camera(Vector4d ep, Vector4d ip, Vector4d dir, Vector4d up, Vector4d fp, Vector4d fd, double d, double fl, double iw, double ih, int pw, int ph, double fstop) {
	_fd = fd;
	_fp = fp;
	if ((_fd.array() != nINF).all()) {
		_fd.normalize();
	}
	if ((ip.array() == nINF).all()) {
		_dir = -dir;
	}
	else {
		_dir = ep - ip;
	}
	if ((dir.array() == nINF).all()) {
		_w = (ep - ip).normalized();
	}
	else {
		_w = -1.0 * (dir).normalized();
	}
	if (fl != nINF && (_fp.array() != nINF).all()) {
		_fp = ep + fl * (_fp - ep).normalized();
	}
	_u = (up.cross3(_w)).normalized(); _u(3) = 0;
	_v = (_w.cross3(_u)).normalized(); _v(3) = 0;
	_width = iw;
	_height = ih;
	if (d == nINF) {
		_d = _dir.norm();
	}
	else {
		_d = d;
	}
	_dir.normalize();
	_e = ep;
	_nx = pw;
	_ny = ph;
	if (fstop == nINF) _size = 0;
	else _size = _d / fstop;
}

/*
IN
vec		p	start point (jittered or not)
i	ith pixel from left
j	jth pixel from bottom
ray		r	reference
*/
void Camera::generateRay(const Vector2d& p, double i, double j, Ray& r) {
	double u = (_width) * (i / _nx - 0.5);
	double v = (_height) * (j / _ny - 0.5);
	Vector2d _p = p * _size;
	Vector4d eye = _e;
	Vector4d dir = -1.0 * _d * _w + u * _u + v * _v;
	Vector4d point = eye + dir;
	if ((_fd.array() != nINF).all() || (_fp.array() != nINF).all()) {
		Vector4d e2p;
		if ((_fd.array() == nINF).all()) {
			_fd = -_w;
			e2p = _fp - eye;
		}
		else if ((_fp.array() == nINF).all()) {
			e2p = -_d * _w;
		}
		double den = _fd.dot(dir);
		double num = _fd.dot(e2p);
		double t = num / den;
		point = eye + t * dir;
	}
	eye = _e + _p[0] * _u + _p[1] * _v;
	dir = point - eye;
	std::vector<double>ref; ref.push_back(1.0);
	std::vector<Vector4d>alph; alph.push_back(Vector4d(0.0, 0.0, 0.0, 0));
	r = Ray(eye, dir, ref, alph, Ray::VIEW);
}

Camera::~Camera() {
	// TODO Auto-generated destructor stub
}
