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
	virtual void sphere(Vector4d pos, double r) {}
	virtual void triangle(Vector4d p1, Vector4d p2, Vector4d p3) {}
	virtual void plane(Vector4d n, Vector4d p) {}
	virtual void circle(Vector4d p, Vector4d n, double r) {}
	virtual void cylinder(double r, double h, char cap) {}
	virtual void cone(double l, double u, char cap) {}
	virtual void torus(double R, double r) {}
	virtual void camera(Vector4d pos, Vector4d at, Vector4d dir, Vector4d up, Vector4d fp, Vector4d fd, double d, double fl, double iw, double ih, int pw, int ph, double size) {}
	virtual void pointLight(Vector4d pos, Vector4d rgb, Vector4d atten, double r) {}
	virtual void directionalLight(Vector4d dir, Vector4d rgb, Vector4d atten) {}
	virtual void spotLight(Vector4d pos, Vector4d dir, double theta, double phi, double p, Vector4d rgb, Vector4d atten, double r) {}
	virtual void ambientLight(Vector4d rgb) {}
	virtual void material(Vector4d amb, Vector4d diff, Vector4d spec, double r, Vector4d refl, double n, Vector4d atten) {}
	virtual void material(string s) {}
	virtual void material(string s, Vector4d amb, Vector4d diff, Vector4d spec, double r, Vector4d refl, double n, Vector4d atten) {}
	virtual void setOption(int option, int setting, int setting2 = 0) {}
	virtual void getObj(const char *file, int smooth) {}
	virtual void invertNormal() {}
	Transform T;
public:
	virtual void parse(const char *file);
	enum { SHADOWS, SAMPLES, REFRACT, STRUCT, RECURSE, DOF, INDIRECT, LIGHTS, ORDER, CIRCULAR, RUSSIAN };
};

#endif
