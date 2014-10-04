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
	bool hit(RayBase& ray, double t0, double t1, hitRecord& rec) const;
	void boundingBox();
	virtual ~Face();
	void setHE(s_ptr<HEdge> e);
	s_ptr<HEdge> getHE() const;
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
	s_ptr<HEdge> getNext() const;
	s_ptr<Vertex> getVertex() const;
private:
	s_ptr<Vertex> _vert;
	w_ptr<HEdge> _next;
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
