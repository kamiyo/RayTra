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
	Vector3d _ep;
	Vector3d _eu;
	Vector3d _ev;
	Vector3d _ew;
	Vector3d _fp;
	Vector3d _fw;
	Vector3d _e2p;
	double _focalDistance;
	double _width;
	double _height;
	double _lensSize;
	int _nx;
	int _ny;
	bool _tilt;
};

#endif /* CAMERA_H_ */
