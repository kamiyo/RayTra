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
#include "PhotonStore.h"
#include "PhotonMap.h"
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
	void initPhotonTracing(double numPhotons);
	Photon emitPhoton() const;
	u_ptr<Photons> tracePhotons(const u_ptr<Group>& s) const;
	Vector3d computeShading(Ray v, double t0, double t1, const u_ptr<Group>& s, const Vector2d& area, int recurs, int refrac) const;
	Vector3d computeShading(Ray v, double t0, double t1, const u_ptr<Group>& s, const Vector2d& area) const;
	Vector3d computeRadianceEstimate(Ray vray, double t0, double t1, const u_ptr<Group>& s) const;
	Vector3d computeRadiance(Ray vray, double t0, double t1, const u_ptr<Group>& s, const Vector2d& area) const;
	void precomputeIrradiance();
	double fresnel(double index1, double index2, double c1, double c2) const;
	double schlicks(double index, double c) const;
	bool refract(const Vector3d& d, const Vector3d& n, double index, double indext, Vector3d& t) const;
	void setRecursDepth(int d);
	Vector3d _amb;
	std::vector<s_ptr<Light> > _l;
	Eigen::VectorXd _lProbs;
	u_ptr<PhotonMap> photonMap;
	u_ptr<PhotonMap> causticMap;
	int _indirect;
	int _recurs;
	int _refraction;
	int _numPhotons;
	int pmNumber;
	double pmRadius;
	bool _russian;
	bool _shadows;
	bool toBreak;
};

#endif /* SHADING_H_ */
