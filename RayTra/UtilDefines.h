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
#include <array>
#include <functional>
#include <algorithm>
#include <stack>
#include <iostream>
#include <random>
#include <memory>
#include <limits>
#include <intrin.h>
#include <immintrin.h>

class Material;
class LightP;
class Surface;


#define _USE_MATH_DEFINES

#define INF std::numeric_limits<double>::infinity()
#define nINF -1 * std::numeric_limits<double>::infinity()
#define EULER 2.718281828459045235360287471352
//#define RAN (double)genrand_real2()
#define RAN genRand_real()
#define RAD sqrt((double)1.0 / M_PI)
#define COSVEC _cosVec
#define DISK toDisk
#define _PLUCKER_

typedef Eigen::Vector2d Vector2d;
typedef Eigen::Vector2i Vector2i;
typedef Eigen::Matrix3d Matrix3d;
typedef Eigen::Matrix4d Matrix4d;
typedef Eigen::Vector3d Vector3d;
typedef Eigen::Vector3i Vector3i;
typedef Eigen::Vector4d Vector4d;
typedef Eigen::AngleAxisd AngleAxisd;
typedef Eigen::Array2d Array2d;
typedef Eigen::Array4d Array4d;
typedef Eigen::Array33d Array33d;
typedef std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Vector2d> > Sampler2d;
typedef std::vector<Eigen::Vector2i, Eigen::aligned_allocator<Vector2i> > Sampler2i;

typedef struct hitRecord {
	double t;
	std::vector<double> ts;
	Vector4d n;
	Material* m;
	LightP* l;
	Surface* s;
} hitRecord;

Vector4d randSphere();

void seedRand();
double genRand_real();
int genRand_int(int x, int y);

Vector4d cosVec(Vector4d a);
Vector4d _cosVec(Vector4d a);

std::istream &operator>>(std::istream &is, Vector3d &f);
std::ostream &operator<<(std::ostream &os, Vector3d &f);

std::istream &operator>>(std::istream &is, Vector4d &f);
std::ostream &operator<<(std::ostream &os, Vector4d &f);

std::ostream &operator<<(std::ostream &os, Sampler2d &s);

// Takes sample in 2-D square to circle
void toDisk(double x, double y, Vector2d& v);
void to_unit_disk(double seedx, double seedy, Vector2d& v);
#endif /* UTILDEFINES_H_ */
