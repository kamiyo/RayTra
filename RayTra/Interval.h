#ifndef INTERVAL_H_
#define INTERVAL_H_

#include "UtilDefines.h"
class Interval;

//typedef std::vector<Interval> Intervals;
typedef std::vector<Interval, std::allocator<Interval> >::iterator IntIter;
typedef std::vector<Interval, std::allocator<Interval> >::const_iterator cIntIter;

class Intervals {
	std::vector<Interval> _int;
public:
	Intervals();
	~Intervals();
	void push_back(Interval i);
	void cat(Intervals i);
	Interval pop_back();
	size_t size();
	void unionize();
	void intersect();
	void intersect(Intervals i);
	void difference(Intervals i);
	IntIter begin();
	IntIter end();
	IntIter erase(cIntIter _where);
	IntIter insert(cIntIter _where, cIntIter _begin, cIntIter _end);
	Interval operator[](int i);
};

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
};

std::ostream &operator<<(std::ostream &out, Interval a);
std::ostream &operator<<(std::ostream &out, Intervals a);

#endif