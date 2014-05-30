/*
 * UtilDefines.h
 *
 *  Created on: Dec 2, 2011
 *      Author: kamiyo
*/

#ifndef UTILDEFINES_H_
#define UTILDEFINES_H_

#include "rand.h"
#include <Eigen/Core>
#include <Eigen/StdVector>
#include <Eigen/Geometry>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <stack>
#include <iostream>
#include <memory>
#include <limits>

class Material;
class LightP;
class Surface;


#define _USE_MATH_DEFINES

#define INF std::numeric_limits<double>::infinity()
#define nINF -1 * std::numeric_limits<double>::infinity()
#define EULER 2.718281828459045235360287471352
#define MAX b[1]
#define MIN b[0]
#define RAN (double)genrand_real2()
#define RAD sqrt((double)1.0 / M_PI)

typedef Eigen::Vector2d Vector2d;
typedef Eigen::Vector2i Vector2i;
typedef Eigen::Matrix3d Matrix3d;
typedef Eigen::Matrix4d Matrix4d;
typedef Eigen::Vector3d Vector3d;
typedef Eigen::Vector4d Vector4d;
typedef Eigen::AngleAxisd AngleAxisd;
typedef Eigen::Array4d Array4d;
typedef Eigen::Array33d Array33d;

typedef struct hitRecord {
	double t;
	Vector3d n;
	Material* m;
	LightP* l;
	Surface* s;
} hitRecord;
Vector3d randSphere();

Vector3d cosVec(Vector3d a);
Vector3d _cosVec(Vector3d a);

std::istream &operator>>(std::istream &is, Vector3d &f);
std::ostream &operator<<(std::ostream &os, Vector3d &f);


// Takes sample in 2-D square to circle
void to_unit_disk(double seedx, double seedy, Vector2d& v);
#endif /* UTILDEFINES_H_ */
