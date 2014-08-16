#ifndef INTERVAL_H_
#define INTERVAL_H_

#include "UtilDefines.h"
class Interval;

typedef std::vector<Interval> Intervals;
/*
class Intervals {
	std::vector<Interval> _int;
public:
	Intervals();

};*/

class Interval
{
public:
	Interval();
	Interval(double min, double max);
	~Interval();
	bool isEmpty();
	Intervals unionize(Interval& other);
	Interval intersect(Interval& other);
	Intervals difference(Interval& diff);
	double _min, _max;
	bool operator==(Interval& rh);
	bool operator!=(Interval& rh);
	static bool intersects(Interval& a, Interval& b);
	static Intervals unionize(Intervals ints);
	static Interval intersect(Intervals ints);
	static Intervals difference(Intervals a, Intervals b);
	static Intervals append(Intervals a, Intervals b);
};

std::ostream &operator<<(std::ostream &out, Interval a);
std::ostream &operator<<(std::ostream &out, Intervals a);

#endif