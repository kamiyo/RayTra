/*
 * vertex.h
 *
 *  Created on: Oct 19, 2011
 *      Author: kamiyo
 */

#ifndef VERTEX_H_
#define VERTEX_H_

#include <vector>
#include <cmath>
#include "UtilDefines.h"
#include "Material.h"

class HEdge;
class Vertex {
public:
	Vertex(Vector4d p);
	virtual ~Vertex();
	void addNormal(Vector4d n);
	Vector4d _n;
	Vector4d _p;
};

#endif /* VERTEX_H_ */
