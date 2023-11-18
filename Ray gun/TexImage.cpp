#include "TexImage.h"
#include "STB.h"
#include<iostream>
#include<filesystem>

TexImage::TexImage():data(nullptr)
{

}

TexImage::~TexImage()
{
	STBI_FREE(data);
}

TexImage::TexImage(const char* filename)
{
	auto currentDir = std::filesystem::current_path();
	std::cout << currentDir.string() + "\\Images\\" + filename << std::endl;
	if(Load(currentDir.string() + "\\Images\\" + filename))return;
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

bool TexImage::Load(std::string filename)
{
	auto n = bytesPerPixel;
	data = stbi_load(filename.c_str(), &imageWidth, &imageHeight, &n, bytesPerPixel);
	if (data == nullptr)
		std::cout << "Failed" << std::endl;
	bytesPerScanline = imageWidth * bytesPerPixel;
	return data != nullptr;

}

int TexImage::GetWidth()const 
{
	return (data == nullptr) ? 0 : imageWidth;
}

int TexImage::GetHeight()const
{
	return (data == nullptr) ? 0 : imageHeight;
}

const unsigned char* TexImage::PixelData(int x, int y) const
{
	static unsigned char magenta[] = {255,0,255};
	if (data == nullptr)
		return magenta;
	x = Clamp(x,0, imageWidth);
	y = Clamp(y,0, imageHeight);
	return data + y * bytesPerScanline + x * bytesPerPixel;
}

int TexImage::Clamp(int x, int low, int high)
{
	if (x < low) return low;
	if (x < high) return x;
	return high - 1;
}