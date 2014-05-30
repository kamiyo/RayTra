/*
 * RayTra.h
 *
 *  Created on: Dec 2, 2011
 *      Author: kamiyo
 */

#ifndef RAYTRA_H_
#define RAYTRA_H_

#include "parse.h"
#include "Group.h"
#include "Sphere.h"
#include "Camera.h"
#include "Triangle.h"
#include "Plane.h"
#include "Cylinder.h"
#include "Circle.h"
#include "Cone.h"
#include "Torus.h"
#include "Shading.h"
#include "Instance.h"
#include "Face.h"
#include "BVH.h"
#include "Hilbert.h"
#include "UtilDefines.h"
#include <OpenEXR/ImfRgbaFile.h>
#include <OpenEXR/ImfStringAttribute.h>
#include <OpenEXR/ImfMatrixAttribute.h>
#include <OpenEXR/ImfArray.h>
#include <map>
#include <fstream>

using namespace std;

class RayTra : public Parser {

	virtual void sphere(Vector3d pos, double r);
	virtual void triangle(Vector3d p1, Vector3d p2, Vector3d p3);
	virtual void plane(Vector3d n, Vector3d p);
	virtual void circle(Vector3d p, Vector3d n, double r);
	virtual void cylinder(double r, double h, char cap);
	virtual void cone(double l, double u, char cap);
	virtual void torus(double R, double r);
	virtual void applyTransform(Surface* s);
	virtual void camera(Vector3d pos, Vector3d dir, Vector3d up, double d, double iw, double ih, int pw, int ph, double size);
	virtual void pointLight(Vector3d pos, Vector3d rgb, Vector3d atten, double r);
	virtual void directionalLight(Vector3d dir, Vector3d rgb, Vector3d atten);
	virtual void spotLight(Vector3d pos, Vector3d dir, double theta, double phi, double p, Vector3d rgb, Vector3d atten, double r);
	virtual void ambientLight(Vector3d rgb);
	virtual void material(Vector3d amb, Vector3d diff, Vector3d spec, double r, Vector3d refl, double n, Vector3d atten);
	virtual void material(string s, Vector3d amb, Vector3d diff, Vector3d spec, double r, Vector3d refl, double n, Vector3d atten);
	virtual void getObj(const char *file, int smooth);
	virtual void setOption(int option, int setting);
	void createFace(int v1, int v2, int v3, int n1, int n2, int n3, int smooth);
	void parseMtl(const char* s);
	void splitStringToInt(const string& s, char d, vector<int>& e);
	void splitStringToNormals(const string& s, char d, vector<int>& e);
	void populateLights();
public:
	RayTra();
	void render(Imf::Array2D<Imf::Rgba>& o);
	virtual void parse(const char* name);
	Group* _surfaces;
	Group* _all;
	Camera* _cam;
	Shading* _shading;
	vector<Material> _m;
	Material* last;
	vector<HEdge*> m_h;
	vector<Vertex*> m_v;
	vector<Face*> m_f;
	vector<Vector3d> m_n;
	map<pair<int, int>, int> pairs;
	map<int, string> m_mtl;
	map<string, Material*> mtlMap;
	int order;
	int width, height;
	int esf;
	int structure;
	int samples;
	int area;
	int field;
	enum { OFF, HARD, SOFTSQUARE, SOFTCIRCLE };
	enum { OFF_0, SQUARE, CIRCLE };
	enum { LINEAR, HILBERT };
	enum { LIST, BoVoH, BiSpPa };
	bool hasnorm;
	bool startMaterial;
	bool circular;
	bool circleLight;
	bool actualLights;

};

#endif /* RAYTRA_H_ */
