/*
 * Shading.h
 *
 *  Created on: Dec 3, 2011
 *      Author: kamiyo
 */

#ifndef SHADING_H_
#define SHADING_H_

#include "LightP.h"
#include "LightD.h"
#include "LightS.h"
#include "Group.h"
#include <limits>
#include <ctime>
#include "Circle.h"
#include "rand.h"
#include "UtilDefines.h"

class Shading {
public:
	Shading();
	virtual ~Shading();
	void addLight(Light* l);
	void addAmbient(Vector4d a);
	Vector4d computeShading(Ray v, double t0, double t1, Group* s, const Vector2d& area, int recurs, int refrac);
	Vector4d computeShading(Ray v, double t0, double t1, Group* s, const Vector2d& area);
	double fresnel(double index1, double index2, double c1, double c2);
	double schlicks(double index, double c);
	bool refract(Vector4d d, Vector4d n, double index, double indext, Vector4d& t);
	void setRecursDepth(int d);
	Vector4d _amb;
	std::vector<Light* > _l;
	int _indirect;
	int _recurs;
	int _refraction;
	bool _russian;
	bool _shadows;
	bool toBreak;
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif /* SHADING_H_ */
