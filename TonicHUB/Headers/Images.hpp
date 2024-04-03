#pragma once
#include <iostream>


struct RData
{
	uint32_t id;
	unsigned char* data;
	int width;
	int height;
	int channels;
};

void InitStaticImages();
uint32_t LoadImageFromHardData(int _width, int _height, int _channels, const char* _hardData);
uint32_t LoadImageWithPath(const char* _path);

RData LoadResizeImage(const char* _path, int _nwidth, int _nheight);

uint32_t GetNoImageId();