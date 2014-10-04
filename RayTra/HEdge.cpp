/*
 * HEdge.cpp
 *
 *  Created on: Oct 19, 2011
 *      Author: kamiyo
 */

#include "Face.h"

HEdge::HEdge(){
	_vert = nullptr;
	_next = w_ptr<HEdge>();
}

void HEdge::setVertex(s_ptr<Vertex> v){
	_vert = v;
}

void HEdge::setNext(s_ptr<HEdge> n) {
	_next = n;
}

s_ptr<HEdge> HEdge::getNext() const {
	if (auto sharedptr = _next.lock()) {
		return sharedptr;
	}
	else {
		return nullptr;
	}
}

s_ptr<Vertex> HEdge::getVertex() const {
	return _vert;
}

HEdge::~HEdge() {
	// TODO Auto-generated destructor stub
}
