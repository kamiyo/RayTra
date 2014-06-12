/*
 * Camera.h
 *
 *  Created on: Dec 2, 2011
 *      Author: kamiyo
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "Ray.h"
#include <iostream>

class Camera {
public:
	Camera(Vector3d ep, Vector3d ip, Vector3d dir, Vector3d up, Vector3d fp, Vector3d fd, double d, double fl, double iw, double ih, int pw, int ph, double fstop);
	void generateRay(const Vector2d& p, double i, double j, Ray& r);
	virtual ~Camera();
	Vector3d _e;
	Vector3d _u;
	Vector3d _v;
	Vector3d _w;
	Vector3d _dir;
	Vector3d _fp;
	Vector3d _fd;
	double _d;
	double _width;
	double _height;
	double _size;
	int _nx;
	int _ny;
};

#endif /* CAMERA_H_ */
