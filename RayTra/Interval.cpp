#include "Interval.h"


Interval::Interval() : _min(INF), _max(nINF)
{
}

Interval::Interval(double min, double max) : _min(min), _max(max) {
}


Interval::~Interval()
{
}

Intervals::Intervals() {}
Intervals::~Intervals() {}

void Intervals::push_back(Interval i) {
	_int.push_back(i);
}

size_t Intervals::size() {
	return _int.size();
}

Interval Intervals::pop_back() {
	Interval temp = _int.back();
	_int.pop_back();
	return temp;
}

std::vector<Interval, std::allocator<Interval> >::iterator Intervals::begin() { return _int.begin(); }
std::vector<Interval, std::allocator<Interval> >::iterator Intervals::end() { return _int.end(); }

void Intervals::cat(Intervals i) {
	_int.insert(end(), i.begin(), i.end());
}

bool compare(Interval& a, Interval& b) {
	return (a._min < b._min);
}

Interval Intervals::operator[](int i) {
	return _int[i];
}

bool Interval::isEmpty() {
	if (_min == INF && _max == nINF) return true;
	else return false;
}

bool Interval::operator==(Interval& rh) {
	if (_min == rh._min && _max == rh._max) {
		return true;
	}
	else {
		return false;
	}
}

bool Interval::operator!=(Interval& rh) {
	if (_min != rh._min || _max != rh._max) {
		return true;
	}
	else {
		return false;
	}
}

Intervals Interval::unionize(Interval& other) {
	Intervals result;
	if (isEmpty()) {
		result.push_back(other);
		return result;
	}
	else if (other.isEmpty()) {
		result.push_back(*this);
		return result;
	}
	else if (intersects(*this, other)) {
		//std::cout << "intersecting" << std::endl;
		Interval newInt(std::min(_min, other._min), std::max(_max, other._max));
		result.push_back(newInt);
		return result;
	}
	else {
		if (_min < other._min) {
			result.push_back(*this);
			result.push_back(other);
			return result;
		}
		else {
			result.push_back(other);
			result.push_back(*this);
			return result;
		}
	}
	return result;
}
Interval Interval::intersect(Interval& other) {
	Interval result;
	if (isEmpty() || other.isEmpty()) {
		return result;
	}
	if (intersects(*this, other)) {
		result = Interval(std::max(_min, other._min), std::min(_max, other._max));
	}
	return result;
}

Intervals Interval::difference(Interval& diff) {
	Intervals result;
	if (isEmpty()) {
		return result;
	}
	else if (diff.isEmpty()) {
		result.push_back(*this);
		return result;
	}
	if (intersects(*this, diff)) {
		if (_min < diff._min) {
			Interval temp(_min, diff._min);
			result.push_back(temp);
		}
		if (_max > diff._max) {
			Interval temp(diff._max, _max);
			result.push_back(temp);
		}
	}
	else {
		result.push_back(*this);
	}
	return result;
}

void Intervals::unionize() {
	std::sort(begin(), end(), compare);
	for (int i = 0; i < size(); i++) {
		for (int j = i + 1; j < size(); j++) {
			Intervals temp = _int[i].unionize(_int[j]);
			//std::cout << ints[i] << " " << ints[j] << std::endl;
			//std::cout << "unioned: " << temp << std::endl;
			if (temp.size() == 1) {
				auto b = begin() + j;
				_int.erase(b);
				_int[i] = temp[0];
				j = i;
			}
		}
	}
}

void Intervals::intersect() {
	for (int i = 1; i < size(); i++) {
		if (_int[0].isEmpty()) {
			break;
		}
		_int[0] = _int[0].intersect(_int[i]);
	}
}

void Intervals::intersect(Intervals i) {

}

Intervals difference(Intervals a, Intervals b) {
	for (int i = 0; i < a.size(); i++) {
		for (int j = 0; j < b.size(); j++) {
			Intervals temp = a[i].difference(b[j]);
			if (temp.size() > 1) {
				a.erase(a.begin() + i);
				a.insert(a.begin() + i, temp.begin(), temp.end());
			}
			else if (temp.size() == 0) {
				a.erase(a.begin() + i);
			}
			else if (temp[0] != a[i]) {
				a[i] = temp[0];
			}
		}
	}
	return a;
}

bool Interval::intersects(Interval& a, Interval& b) {
	if (a._max < b._min || b._max < a._min)
		return false;
	else return true;
}

std::ostream &operator<< (std::ostream &o, Interval a) {
	if (a.isEmpty()) {
		return o << "()";
	}
	return o << "(" << a._min << ", " << a._max << ")";
}

std::ostream &operator<< (std::ostream &o, Intervals a) {
	o << "{";
	for (int i = 0; i < a.size(); i++) {
		o << a[i];
		if (i < a.size() - 1) {
			o << ", ";
		}
	}
	return o << "}";
}