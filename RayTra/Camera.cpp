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
double	_d		distance (focal length)
		_width	image width
		_height	image height
		_size	lens size
int		_nx		width (pixels)
		_ny		height (pixels)

*/

/*
IN
vec		pos		eye-coord
		dir		eye-dir
		up		up-vector
double	d		distance (focal length)
		iw		width (units)
		ih		height (units)
		fstop	fstop (distance / size)
int		pw		width (pixels)
		ph		height (pixels)
*/
Camera::Camera(Vector3d pos, Vector3d dir, Vector3d up, double d, double iw, double ih, int pw, int ph, double fstop) {
	_w = -1.0 * (dir).normalized();
	_u = (up.cross(_w)).normalized();
	_v = (_w.cross(_u)).normalized();
	_width = iw;
	_height = ih;
	_d = d;
	_e = pos;
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
void Camera::generateRay(Vector2d p, double i, double j, Ray& r) {
	double u = (_width) * (i / _nx - 0.5);
	double v = (_height) * (j / _ny - 0.5);
	Vector2d _p = p * _size;
	Vector3d eye = _e;
	Vector3d dir = -1.0 * _d * _w + u * _u + v * _v;
	Vector3d point = eye + dir;
	eye = _e + _p[0] * _u + _p[1] * _v;
	dir = point - eye;
	std::vector<double>ref; ref.push_back(1.0);
	std::vector<Vector3d>alph; alph.push_back(Vector3d(0.0, 0.0, 0.0));
	r = Ray(eye, dir, ref, alph, Ray::VIEW);
}

Camera::~Camera() {
	// TODO Auto-generated destructor stub
}
