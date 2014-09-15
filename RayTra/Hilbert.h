#ifndef HILBERT_H_
#define HILBERT_H_
#include "UtilDefines.h"

class Hilbert {
public:
	Hilbert(int x, int y);
	~Hilbert();
	Sampler2i& getPoints();
	int size();
private:
	Sampler2i _points;
	int _size;
	int _x, _y;
	static const int N = 0, E = 1, S = 2, W = 3;
	int nX, nY;
	void move(int d);
	void hilbert(int i, int front, int right, int behind, int left);
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif