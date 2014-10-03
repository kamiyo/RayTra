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
	Face(s_ptr<Material> m);
	bool hit(RayBase& ray, double t0, double t1, hitRecord& rec);
	void boundingBox();
	virtual ~Face();
	void setHE(s_ptr<HEdge> e);
	s_ptr<HEdge> getHE();
	Vector3d _normal;
	bool smooth;
private:
	s_ptr<HEdge> _e;
};

class HEdge {
public:
	HEdge();
	virtual ~HEdge();
	void setNext(s_ptr<HEdge> n);
	void setVertex(s_ptr<Vertex> v);
	void setFace(s_ptr<Face> f);
	s_ptr<HEdge> getNext();
	s_ptr<Vertex> getVertex();
	s_ptr<Face> getFace();
private:
	s_ptr<Vertex> _vert;
	s_ptr<HEdge> _next;
	s_ptr<Face> _face;
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
