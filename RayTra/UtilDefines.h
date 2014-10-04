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

class Material;
class LightP;
class Surface;


#define _USE_MATH_DEFINES

#define INF std::numeric_limits<double>::infinity()
#define nINF -1 * std::numeric_limits<double>::infinity()
#define EULER 2.718281828459045235360287471352
//#define RAN (double)genrand_real2()
#define RAN genRand_real()
#define GAUSS genRand_gauss()
#define RAD sqrt((double)1.0 / M_PI)
#define COSVEC cosVec
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
typedef Eigen::Matrix<Vector2d, Eigen::Dynamic, Eigen::Dynamic> Sampler2d;
typedef Eigen::Matrix<Vector2i, Eigen::Dynamic, Eigen::Dynamic> Sampler2i;

template <typename Object>
using u_ptr = std::unique_ptr<Object>;

template <typename Object>
using s_ptr = std::shared_ptr<Object>;

template <typename Object>
using w_ptr = std::weak_ptr<Object>;

typedef struct hitRecord {
	double t;
//	std::vector<double> ts;
	Vector3d n;
	s_ptr<Material> m;
	s_ptr<LightP> l;
	s_ptr<Surface> s;
} hitRecord;

Vector3d randSphere();

void seedRand();
double genRand_real();
double genRand_gauss();
double genRand_real(double x, double y);
int genRand_int(int x, int y);

Vector3d cosVec(Vector3d a);
Vector3d _cosVec(Vector3d a);
Vector3d cosVec(Vector3d a, double p);


std::istream &operator>>(std::istream &is, Vector3d &f);
std::ostream &operator<<(std::ostream &os, Vector3d &f);

std::ostream &operator<<(std::ostream &os, Sampler2d &s);
std::ostream &operator<<(std::ostream &os, Sampler2i &s);


// Takes sample in 2-D square to circle
void toDisk(double x, double y, Vector2d& v);
void toDisk(Vector2d& v);
void to_unit_disk(double seedx, double seedy, Vector2d& v);
#endif /* UTILDEFINES_H_ */
