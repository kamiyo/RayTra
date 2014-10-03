#ifndef PARSE_H_
#define PARSE_H_

#include <cstdlib>
#include <string>
#include <iostream>
#include "UtilDefines.h"
#include "Transform.h"

using namespace std;

class Parser {
protected:
	virtual void sphere(Vector3d pos, double r) {}
	virtual void triangle(Vector3d p1, Vector3d p2, Vector3d p3) {}
	virtual void plane(Vector3d n, Vector3d p) {}
	virtual void circle(Vector3d p, Vector3d n, double r) {}
	virtual void cylinder(double r, double h, char cap) {}
	virtual void cone(double l, double u, char cap) {}
	virtual void torus(double R, double r) {}
	virtual void createCamera(Vector3d pos, Vector3d at, Vector3d dir, Vector3d up, Vector3d fp, Vector3d fd, double d, double fl, double iw, double ih, int pw, int ph, double size) {}
	virtual void pointLight(Vector3d pos, Vector3d rgb, Vector3d atten, double r) {}
	virtual void directionalLight(Vector3d dir, Vector3d rgb, Vector3d atten) {}
	virtual void spotLight(Vector3d pos, Vector3d dir, double theta, double phi, double p, Vector3d rgb, Vector3d atten, double r) {}
	virtual void ambientLight(Vector3d rgb) {}
	virtual void material(Vector3d amb, Vector3d diff, Vector3d spec, double r, Vector3d refl, double n, Vector3d atten) {}
	virtual void material(string s) {}
	virtual void material(string s, Vector3d amb, Vector3d diff, Vector3d spec, double r, Vector3d refl, double n, Vector3d atten) {}
	virtual void setOption(int option, int setting, int setting2 = 0) {}
	virtual void getObj(const char *file, int smooth) {}
	virtual void invertNormal() {}
	Transform T;
public:
	virtual void parse(const char *file);
	const static int SHADOWS = 0, SAMPLES = 1, REFRACT = 2, STRUCT = 3, RECURSE = 4, DOF = 5, INDIRECT = 6, ACTUALLIGHTS = 7, ORDER = 8, RUSSIAN = 9;
};

#endif
