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
	Camera(Vector4d ep, Vector4d ip, Vector4d dir, Vector4d up, Vector4d fp, Vector4d fd, double d, double fl, double iw, double ih, int pw, int ph, double fstop);
	void generateRay(const Vector2d& p, double i, double j, Ray& r);
	virtual ~Camera();
	Vector4d _e;
	Vector4d _u;
	Vector4d _v;
	Vector4d _w;
	Vector4d _dir;
	Vector4d _fp;
	Vector4d _fd;
	double _d;
	double _width;
	double _height;
	double _size;
	int _nx;
	int _ny;
};

#endif /* CAMERA_H_ */
