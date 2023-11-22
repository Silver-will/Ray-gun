#ifndef INTERVAL_H
#define INTERVAL_H
#include"Common.h"
using namespace::Common;
struct Interval
{
	double min, max;

	Interval();
	Interval(double _min, double _max);
	Interval(const Interval& a, const Interval& b);

	bool Contains(double x) const;
	bool Surrounds(double x) const;

	double Clamp(double x) const;
	double Size() const;

	Interval Expand(double delta)const;
	static const Interval empty, universe;
	Interval operator+(double displacement);
	Interval operator+(Interval& lval);
};

const Interval  Interval::empty = Interval(infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, infinity);
#endif

