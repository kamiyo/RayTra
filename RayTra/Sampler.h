#ifndef __SAMPLER_H
#define __SAMPLER_H

#include "UtilDefines.h"

class Sampler
{
public:
	Sampler();
	Sampler(int dim_x, int dim_y);
	Sampler(int dim_x, int dim_y, int type, int shape, int order, int algo, bool center);
	~Sampler();
	Sampler2d genPoints();
	static void shuffle(Sampler2d& samples, bool together = false);
	//static void shuffle_correlated(Sampler2d& samples, int m, int n, bool together = false);
	Sampler2i points;
	Vector2d xy;
	int _x, _y, _type, _shape, _order, _algo;
	bool _center;
	bool _zero;
	enum {LINEAR, HILBERT};
	enum {INTEGRAL, FRACTIONAL};
	enum {SQUARE, CIRCLE};
	enum {CENTER, RANDOM, STRATIFIED, NROOKS};
	
};

#endif