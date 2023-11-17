#include "Texture.h"
#include "Interval.h"

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

ImageTexture::ImageTexture(const char* filename) : image(filename)
{

}

Color ImageTexture::Value(double u, double v, const Point& p)const
{
	if(image.GetHeight() <= 0)
		return Color(0,1,1);
	// Clamp input texture coordinates to [0,1] x [1,0]
	u = Interval(0,1).clamp	(u);
	v =	1.0 - Interval(0,1).clamp(v);
	// Flip V to image coordinates
	auto i = static_cast<int>(u * image.GetWidth());
	auto j = static_cast<int>(v * image.GetHeight());
	auto pixel = image.PixelData(i, j);
	auto color_scale = 1.0/255.0;
	return Color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
}