/*
 * Camera.cpp
 *
 *  Created on: Dec 2, 2011
 *      Author: kamiyo
 */

#include "Camera.h"
/*
VARS
vec		_eye		eye pos
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
dir		image plane normal (= viewing direction if no shift)
up		up-vector
double	d		distance (to image)
iw		width (units)
ih		height (units)
fstop	fstop (distance / size)
int		pw		width (pixels)
ph		height (pixels)
*/
Camera::Camera(Vector3d ep, Vector3d ip, Vector3d dir, Vector3d up, Vector3d fp, Vector3d fd, double d, double fl, double iw, double ih, int pw, int ph, double fstop) {

	// eye uvw
	if ((ip.array() == nINF).all()) {
		if ((dir.array() == nINF).all()) {
			std::cerr << "must supply either image-plane location or view direction." << std::endl;
			exit(1);
		}
		else {
			_ew = -dir;
		}
	}
	else {
		_ew = ep - ip;
	}

	if ((fd.array() != nINF).all()) {
		_fw = -fd.normalized();
		_tilt = true;
	}
	else {
		_fw = _ew.normalized();
	}

	if ((fp.array() != nINF).all()) {
		_fp = fp;
	}
	else {
		_fp = ep - _ew;
	}

	_focalDistance = (d == nINF) ? _ew.norm() : d;

	_ew.normalize();
	_eu = (up.cross(_ew)).normalized();
	_ev = (_ew.cross(_eu)).normalized();
	_ep = ep;

	_e2p = _fp - _ep;
	_width = iw;
	_height = ih;

	_nx = pw;
	_ny = ph;

	_lensSize = (fstop == nINF) ? 0 : (_focalDistance / fstop);
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
	Vector2d sample = p * _lensSize;
	Vector3d eye = _ep;
	Vector3d dir = _focalDistance * _e2p + u * _eu + v * _ev;
	Vector3d point = eye + dir;
	if (_tilt) {
		double t = _fw.dot(_e2p) / _fw.dot(dir);
		point = eye + t * dir;
	}
	eye = _ep + sample[0] * _eu + sample[1] * _ev;
	dir = point - eye;
	std::vector<double>ref; ref.push_back(1.0);
	std::vector<Vector3d>alph; alph.push_back(Vector3d(0.0, 0.0, 0.0));
	r = Ray(eye, dir, ref, alph, Ray::VIEW);
}

Camera::~Camera() {
	// TODO Auto-generated destructor stub
}
