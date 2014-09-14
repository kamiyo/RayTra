/*
 * vertex.cpp
 *
 *  Created on: Oct 19, 2011
 *      Author: kamiyo
 */

#include "Face.h"

Vertex::Vertex(Vector4d p) {
	_p = p;
	_n.setZero();
}

void Vertex::addNormal(Vector4d n) {
	_n += n;
}

Vertex::~Vertex() {
	// TODO Auto-generated destructor stub
}
