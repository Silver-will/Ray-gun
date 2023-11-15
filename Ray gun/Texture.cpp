#include "Texture.h"

SolidColor::SolidColor(Color c) :ColorVal{c}
{

}

SolidColor::SolidColor(double red, double green, double blue) : SolidColor(Color(red, green, blue))
{

}

Color SolidColor::Value(double u, double v, const Point& p)const
{
	return ColorVal;
}

CheckerTexture::CheckerTexture(double s, std::shared_ptr<Texture> even_, std::shared_ptr<Texture> odd_)
	: invScale(s/1.0), even{even_},odd{odd_}
{

}

CheckerTexture::CheckerTexture(double s, Color c1, Color c2)
	:invScale(s/1.0),even(std::make_shared<SolidColor>(c1)), 
		odd(std::make_shared<SolidColor>(c2))
{

}

Color CheckerTexture::Value(double u, double v, const Point& p)const
{
	auto xInteger = static_cast<int>(std::floor(invScale * p.x));
	auto yInteger = static_cast<int>(std::floor(invScale * p.y));
	auto zInteger = static_cast<int>(std::floor(invScale * p.z));
	bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;
	return isEven ? even->Value(u, v, p) : odd->Value(u, v, p);
}