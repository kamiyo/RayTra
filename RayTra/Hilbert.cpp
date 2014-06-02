#include "Hilbert.h"

Hilbert::Hilbert(int x, int y) : _x(x), _y(y) {
	int m = (x > y) ? x : y;
	int nOrder = 1;
	int temp = 2;
	while (temp < m) {
		temp = temp << 1;
		nOrder++;
	}
	//int check = 1;
	//while ((1<<check)<m) check++;
	//if (nOrder != check) exit(0);
	_points.resize(x * y);
	_size = 0;
	nX = nY = 0;
	hilbert(nOrder, N, E, S, W);
}

Sampler2i& Hilbert::getPoints() {
	return _points;
}

int Hilbert::size() {
	return _size;
}


void Hilbert::move(int d) {
	switch(d)
	{
		case N: nY--; break;
		case E: nX++; break;
		case S: nY++; break;
		case W: nX--; break;
	}
}

Hilbert::~Hilbert() {}

void Hilbert::hilbert(int i, int front, int right, int behind, int left) {
	if (i == 0) {
		if (nX < _x && nY < _y)
		{
			_points[_size++] = Vector2i(nX, nY);
		}
	}
	else {
		hilbert(i-1, left, behind, right, front);
		move(right);
		hilbert(i-1, front, right, behind, left);
		move(behind);
		hilbert(i-1, front, right, behind, left);
		move(left);
		hilbert(i-1, right, front, left, behind);
	}
}