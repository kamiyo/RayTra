/*
 * face.h
 *
 *  Created on: Oct 19, 2011
 *      Author: kamiyo
 */

#ifndef FACE_H_
#define FACE_H_

class HEdge;
class Vertex;

#include "Surface.h"
#include <vector>
#include <cmath>

class Face : public Surface {
public:
	Face(Material* m);
	bool hit(Ray& ray, double t0, double t1, hitRecord& rec);
	void boundingBox();
	virtual ~Face();
	void setHE(HEdge* e);
	HEdge* getHE();
	Vector3d _normal;
	bool smooth;
private:
	HEdge* _e;
};

class HEdge {
public:
	HEdge();
	virtual ~HEdge();
	void setNext(HEdge* n);
	void setVertex(Vertex* v);
	void setFace(Face* f);
	HEdge* getNext();
	Vertex* getVertex();
	Face* getFace();
private:
	Vertex* _vert;
	HEdge* _next;
	Face* _face;
};

class Vertex {
public:
	Vertex(Vector3d p);
	virtual ~Vertex();
	void addNormal(Vector3d n);
	Vector3d _n;
	Vector3d _p;
};

#endif /* FACE_H_ */
