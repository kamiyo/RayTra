/*
 * HEdge.cpp
 *
 *  Created on: Oct 19, 2011
 *      Author: kamiyo
 */

#include "Face.h"

HEdge::HEdge(){
	_vert = NULL;
	_next = NULL;
	_face = NULL;
}

void HEdge::setVertex(Vertex* v){
	_vert = v;
}

void HEdge::setNext(HEdge* n) {
	_next = n;
}

void HEdge::setFace(Face* f) {
	_face = f;
}

HEdge* HEdge::getNext() {
	return _next;
}

Vertex* HEdge::getVertex() {
	return _vert;
}

Face* HEdge::getFace() {
	return _face;
}

HEdge::~HEdge() {
	// TODO Auto-generated destructor stub
}
