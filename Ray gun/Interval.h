#ifndef INTERVAL_H
#define INTERVAL_H
#include"Common.h"
using namespace::Common;
struct Interval
{
	double min, max;

	Interval();
	Interval(double _min, double _max);

	bool contains(double x) const;
	bool surrounds(double x) const;

	double clamp(double x) const;

	static const Interval empty, universe;
};

const static Interval empty(infinity, -infinity);
const static Interval universe(-infinity, infinity);
#endif

