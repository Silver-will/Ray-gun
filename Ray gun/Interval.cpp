#include "Interval.h"

Interval::Interval(): min{-infinity}, max{infinity}
{

}

Interval::Interval(const Interval& a, const Interval& b)
	: min(fmin(a.min,b.min)), max(fmax(a.max,b.max))
{

}

Interval::Interval(double _min, double _max)
	:  min{_min}, max{_max}
{

}

bool Interval::Contains(double x) const
{
	return min <= x && x <= max;
}

bool Interval::Surrounds(double x) const
{
	return min < x && x < max;
}

double Interval::Clamp(double x) const
{
	if (x < min)return min;
	if (x > max)return max;
	return x;
}

double Interval::Size() const
{
	return max - min;
}

Interval Interval::Expand(double delta)const
{
	auto padding = delta / 2;
	return Interval(min - padding, max + padding);
}