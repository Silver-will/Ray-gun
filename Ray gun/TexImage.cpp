#include "ImageLoader.h"
#include<iostream>
using namespace std::string_literals;
ImageLoader::ImageLoader():data(nullptr)
{

}

ImageLoader::~ImageLoader()
{
	STBI_FREE(data);
}

ImageLoader::ImageLoader(const char* filename)
{
	auto filename = std::string(filename);
	auto imagedir = getenv("Images");
	
	if(imagedir && Load(std::string(imagedir) +"/"+ filename))
		return;
	if(Load(filename))return;
	if(Load("images/"s + filename) )return;
	if(Load("../images/"s + filename))return;
	if(Load("../../images/"s + filename))return;
	if(Load("../../../images/"s + filename))return;
	if(Load("../../../../images/"s + filename))return;
	if(Load("../../../../../images/"s + filename))return;
	if(Load("../../../../../../images/"s + filename))return;
	std::cerr <<"ERROR: Could not load image file '"<< filename <<"'.\n";
}

bool ImageLoader::Load(std::string filename)
{
	auto n = bytesPerPixel;
	data = stbi_load(filename.c_str(), &imageWidth, &imageHeight, &n, bytesPerPixel);
	bytesPerScanline = imageWidth * bytesPerPixel;
	return data != nullptr;

}

int ImageLoader::GetWidth()const 
{
	return (data == nullptr) ? 0 : imageWidth;
}

int ImageLoader::GetHeight()const
{
	return (data == nullptr) ? 0 : imageHeight;
}

const unsigned char* ImageLoader::PixeLData(int x, int y) const
{
	static unsigned char magenta[] = {255,0,255};
	if (data == nullptr)
		return magenta;
	x = Clamp(x,0, imageWidth);
	y = Clamp(y,0, imageHeight);
	return data + y * bytesPerScanline + x * bytesPerPixel;
}

int ImageLoader::Clamp(int x, int low, int high)
{
	if (x < low) return low;
	if (x >= high) return high - 1;
	return x;
}