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
#include "PhotonMap.h"
#include "Instance.h"
#include "Face.h"
#include "BVH.h"
#include "Hilbert.h"
#include "Sampler.h"
#include "UtilDefines.h"
#include "Interval.h"
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
	virtual void applyTransform(s_ptr<Surface> &s);
	virtual void createCamera(Vector3d pos, Vector3d at, Vector3d dir, Vector3d up, Vector3d fp, Vector3d fd, double d, double fl, double iw, double ih, int pw, int ph, double size);
	virtual void pointLight(Vector3d pos, Vector3d rgb, Vector3d atten, double r);
	virtual void directionalLight(Vector3d dir, Vector3d rgb, Vector3d atten);
	virtual void spotLight(Vector3d pos, Vector3d dir, double theta, double phi, double p, Vector3d rgb, Vector3d atten, double r);
	virtual void ambientLight(Vector3d rgb);
	virtual void material(Vector3d amb, Vector3d diff, Vector3d spec, double r, Vector3d refl, double n, Vector3d atten);
	virtual void material(string s, Vector3d amb, Vector3d diff, Vector3d spec, double r, Vector3d refl, double n, Vector3d atten);
	virtual void material(string s);
	virtual void getObj(const char *file, int smooth);
	virtual void setOption(int option, int setting, int setting2 = 0);
	virtual void invertNormal();
	void createFace(int v1, int v2, int v3, int n1, int n2, int n3, int smooth);
	void parseMtl(const char* s);
	void splitStringToInt(const string& s, char d, vector<int>& e);
	void splitStringToNormals(const string& s, char d, vector<int>& e);
	void populateLights();
public:
	RayTra();
	~RayTra();
	void render(Imf::Array2D<Imf::Rgba>& o);
	virtual void parse(const char* name);
	s_ptr<Group> surfaces;		// does not include planes
	s_ptr<Group> allSurfaces;		// includes planes
	u_ptr<Camera> camera;
	u_ptr<Shading> shading;
	vector<Material> materials;
	s_ptr<Material> prevMaterial;
	vector<s_ptr<HEdge> > m_h;
	vector<s_ptr<Vertex> > m_v;
	vector<s_ptr<Face> > m_f;
	vector<Vector3d> m_n;
	map<pair<int, int>, int> pairs;
	map<int, string> mtl;
	map<string, s_ptr<Material> > mtlMap;
	int renderOrder;			//Render Order: Linear or Hilbert
	int width, height;
	int esf;
	int accelerationStructure;	// List or BVH (for now)
	int sampleType;				// center, random, stratified, nrooks, nrooks correlated
	int numSamples;				// number of samples for each pixel, will be squared.
	int shadowSetting;			// Off, hard, soft square, soft circle
	int dofSetting;				// DoF: Off, Square, or Circle
	static const int OFF = 0, HARD = 1, SOFTSQUARE = 2, SOFTCIRCLE = 3;
	static const int SQUARE = 1, CIRCLE = 2;
	static const int LINEAR = 0, HILBERT = 1;
	static const int LIST = 0, BoVoH = 1;
	static const int CENTER = 0, RANDOM = 1, STRATIFIED = 2, NROOKS = 3, NROOKSCORR = 4;
	bool hasNorm;
	bool startMaterial;
	bool circleLight;
	bool actualLights;

};

#endif /* RAYTRA_H_ */
