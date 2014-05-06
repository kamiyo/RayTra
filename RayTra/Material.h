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
	Material(Vector3d amb, Vector3d diff, Vector3d spec, double r, Vector3d ref, double index, Vector3d atten);
	virtual ~Material();
	Vector3d kd;
	Vector3d ks;
	Vector3d ki;
	Vector3d a;
	Vector3d ka;
	double p;
	double n;
};

#endif /* MATERIAL_H_ */
