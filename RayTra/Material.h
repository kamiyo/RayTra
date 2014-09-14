/*
 * Material.h
 *
 *  Created on: Dec 2, 2011
 *      Author: kamiyo
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_
#include "UtilDefines.h"

class Material {
public:
	Material(Vector4d amb, Vector4d diff, Vector4d spec, double r, Vector4d ref, double index, Vector4d atten);
	virtual ~Material();
	Vector4d kd;
	Vector4d ks;
	Vector4d ki;
	Vector4d a;
	Vector4d ka;
	double p;
	double n;
};

#endif /* MATERIAL_H_ */
