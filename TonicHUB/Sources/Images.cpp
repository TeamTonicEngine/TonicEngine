#include "Images.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.hpp>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <STB/stb_image_resize2.hpp>

#include <glad/glad.hpp>

uint32_t noImageId;

void InitStaticImages()
{
    
    noImageId = LoadImageWithPath("StaticAssets\\Images\\noimage.png");
}

uint32_t LoadImageFromHardData(int _width, int _height, int _channels, const char* _hardData)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    int si = strlen(_hardData);

    std::cout << _hardData << std::endl;

    if(_channels == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _hardData);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, _hardData);

    glGenerateMipmap(GL_TEXTURE_2D);

    glSamplerParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return textureID;
}
uint32_t LoadImageWithPath(const char* _path)
{
    int width, height, channels;
    unsigned char* imageData = stbi_load(_path, &width, &height, &channels, 0);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if(channels == 4)
       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    else
       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

    
    glGenerateMipmap(GL_TEXTURE_2D);

    glSamplerParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    stbi_image_free(imageData);
	return textureID;
}

RData LoadResizeImage(const char* _path, int _nwidth, int _nheight)
{
    //Not Working as intended

    static GLuint textureID;
    static int channels;
    static bool valid = false;
    static std::string prePath;
    static unsigned char* RimageData = nullptr;

    if (strcmp(prePath.c_str(), _path) == 0)
    {
        if (valid)
            return { textureID, RimageData, _nwidth, _nheight, channels };
        return {GetNoImageId(), nullptr,0,0,0 };
    }
    else
        prePath = _path;



    int width, height;
    static int preChannels = -1;
    unsigned char* imageData = stbi_load(_path, &width, &height, &channels, 0);
    if (!imageData)
    {
        valid = false;
        return { GetNoImageId(), nullptr,0,0,0 };
    }
    valid = true;

    if (channels != preChannels)
    {
        if(RimageData)
            delete[] RimageData;
        RimageData = new unsigned char[_nwidth * _nheight * channels];
        preChannels = channels;
    }

    stbir_resize_uint8_linear(imageData, width, height, 0, RimageData, _nwidth, _nheight, 0, static_cast<stbir_pixel_layout>(channels));
   
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (channels == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _nwidth, _nheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, RimageData);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _nwidth, _nheight, 0, GL_RGB, GL_UNSIGNED_BYTE, RimageData);

    glGenerateMipmap(GL_TEXTURE_2D);

    glSamplerParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(imageData);
    return { textureID, RimageData,_nwidth,_nheight, channels };
}

uint32_t GetNoImageId()
{
    return noImageId;
}