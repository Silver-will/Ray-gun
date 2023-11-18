#ifndef TEXIMAGE_H
#define TEXIMAGE_H

#include<string>
#include<iostream>
using namespace std::string_literals;

struct TexImage
{
	TexImage();
	TexImage(const char* filename);
	~TexImage();
	bool Load(const std::string filename);
	int GetWidth() const;
	int GetHeight()const;
	const unsigned char* PixelData(int x, int y)const;

private:
	const int bytesPerPixel = 3;
	unsigned char* data;
	int imageWidth, imageHeight;
	int bytesPerScanline;

	static int Clamp(int x, int low, int high);
};


#endif

