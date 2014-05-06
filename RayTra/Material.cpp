/*
 * Material.cpp
 *
 *  Created on: Dec 2, 2011
 *      Author: kamiyo
 */

#include "Material.h"

Material::Material(Vector3d amb, Vector3d diff, Vector3d spec, double r, Vector3d ref, double index, Vector3d atten) {
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
