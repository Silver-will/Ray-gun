#ifndef TEXTURE_H
#define TEXTURE_H
#include"Typedefs.h"
#include"TexImage.h"
#include"Perlin.h"
#include<memory>
#include<utility>

struct Texture
{
	virtual ~Texture() = default;
	virtual Color Value(double u, double v, const Point& p)const = 0;

};

struct SolidColor : public Texture
{
	SolidColor(Color c);
	SolidColor(double red, double green, double blue);
	Color Value(double u, double v, const Point& p)const override;

private:
	Color ColorVal;
};

struct CheckerTexture : public Texture
{
	CheckerTexture(double s, std::shared_ptr<Texture> even_, std::shared_ptr<Texture> odd_);
	CheckerTexture(double s, Color c1, Color c2);
	Color Value(double u, double v, const Point& p)const override;

private:
	double invScale;
	std::shared_ptr<Texture> even;
	std::shared_ptr<Texture> odd;
};

struct ImageTexture : public Texture
{
	ImageTexture(){}
	ImageTexture(const char* filename);
	bool LoadImage(const char* filename);
	Color Value(double u, double v, const Point& p)const override;

private:
	TexImage image;
};

struct NoiseTexture : public Texture
{
	NoiseTexture() {}

	NoiseTexture(double sc) : scale(sc) {}
	Color Value(double u, double v, const Point& p)const override;

private:
	Perlin noise{};
	double scale{};
};
#endif

