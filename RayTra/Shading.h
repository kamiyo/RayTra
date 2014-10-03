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
#include "Photon.h"
#include <limits>
#include <ctime>
#include "Circle.h"
#include "rand.h"
#include "UtilDefines.h"

class Shading {
public:
	Shading();
	virtual ~Shading();
	void addLight(s_ptr<Light> l);
	void addAmbient(Vector3d a);
	void initPhotonTracing();
	Photon emitPhoton();
	Photons* tracePhotons(Group *s);
	Vector3d computeShading(Ray v, double t0, double t1, s_ptr<Group> s, const Vector2d& area, int recurs, int refrac);
	Vector3d computeShading(Ray v, double t0, double t1, s_ptr<Group> s, const Vector2d& area);
	double fresnel(double index1, double index2, double c1, double c2);
	double schlicks(double index, double c);
	bool refract(Vector3d d, Vector3d n, double index, double indext, Vector3d& t);
	void setRecursDepth(int d);
	Vector3d _amb;
	std::vector<s_ptr<Light> > _l;
	Eigen::ArrayXd _lProbs;
	int _indirect;
	int _recurs;
	int _refraction;
	int _numPhotons;
	bool _russian;
	bool _shadows;
	bool toBreak;
};

#endif /* SHADING_H_ */
