#ifndef PARSE_H_
#define PARSE_H_

#include <cstdlib>
#include <cstring>
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
	virtual void cone(double h, char cap) {}
	virtual void torus(double R, double r) {}
	virtual void camera(Vector3d pos, Vector3d dir, Vector3d up, double d, double iw, double ih, int pw, int ph, double size) {}
	virtual void pointLight(Vector3d pos, Vector3d rgb, Vector3d atten, double r) {}
	virtual void directionalLight(Vector3d dir, Vector3d rgb, Vector3d atten) {}
	virtual void spotLight(Vector3d pos, Vector3d dir, double theta, double phi, double p, Vector3d rgb, Vector3d atten, double r) {}
	virtual void ambientLight(Vector3d rgb) {}
	virtual void material(Vector3d amb, Vector3d diff, Vector3d spec, double r, Vector3d refl, double n, Vector3d atten) {}
	virtual void material(string s, Vector3d amb, Vector3d diff, Vector3d spec, double r, Vector3d refl, double n, Vector3d atten) {}
	virtual void setOption(int option, int setting) {}
	virtual void getObj(const char *file, int smooth) {}
	Transform T;
public:
	virtual void parse(const char *file);
	enum { SHADOWS, SAMPLES, REFRACT, STRUCT, RECURSE, DOF, INDIRECT, LIGHTS, ORDER, CIRCULAR };
};

#endif
