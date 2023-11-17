#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#ifdef _MSC_VER
	#pragma warning(push,0); 
#endif
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb_image.h>

#include<string>
struct ImageLoader
{
	ImageLoader();
	ImageLoader(const char* filename);
	~ImageLoader();
	bool Load(const std::string filename);
	int GetWidth() const;
	int GetHeight()const;
	const unsigned char* PixeLData(int x, int y)const;

private:
	const int bytesPerPixel = 3;
	unsigned char* data;
	int imageWidth, imageHeight;
	int bytesPerScanline;

	static int Clamp(int x, int low, int high);
};

#ifdef _MSC_VER
	#pragma warning (pop)
#endif
#endif

