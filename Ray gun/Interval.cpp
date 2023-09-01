#include "Interval.h"

Interval::Interval(): min{-infinity}, max{infinity}
{

}

Interval::Interval(double _min, double _max)
	:  min{_min}, max{_max}
{

}

bool Interval::contains(double x) const
{
	return min <= x && x <= max;
}

bool Interval::surrounds(double x) const
{
	return min < x && x < max;
}

double Interval::clamp(double x) const
{
	if (x < min)return min;
	if (x > max)return max;
	return x;
}