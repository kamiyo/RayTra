/*
 * Material.cpp
 *
 *  Created on: Dec 2, 2011
 *      Author: kamiyo
 */

#include "Material.h"

Material::Material(Vector4d amb, Vector4d diff, Vector4d spec, double r, Vector4d ref, double index, Vector4d atten) {
	kd = diff;
	ks = spec;
	p = r;
	ki = ref;
	n = index;
	ka = amb;
	a = atten;
}

Material::~Material() {
	// TODO Auto-generated destructor stub
}
