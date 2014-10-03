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

void HEdge::setVertex(s_ptr<Vertex> v){
	_vert = v;
}

void HEdge::setNext(s_ptr<HEdge> n) {
	_next = n;
}

void HEdge::setFace(s_ptr<Face> f) {
	_face = f;
}

s_ptr<HEdge> HEdge::getNext() {
	return _next;
}

s_ptr<Vertex> HEdge::getVertex() {
	return _vert;
}

s_ptr<Face> HEdge::getFace() {
	return _face;
}

HEdge::~HEdge() {
	// TODO Auto-generated destructor stub
}
