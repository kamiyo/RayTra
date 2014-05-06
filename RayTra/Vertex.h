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
	Vertex(Vector3d p);
	virtual ~Vertex();
	void addNormal(Vector3d n);
	Vector3d _n;
	Vector3d _p;
};

#endif /* VERTEX_H_ */
