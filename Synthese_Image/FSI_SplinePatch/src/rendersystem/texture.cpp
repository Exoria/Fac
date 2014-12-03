#include "texture.h"
#include <iostream>

namespace rendersystem {

Texture::Texture (std::string filename){
    QImage img(filename.c_str());
    std::cerr << filename << " : " << img.width() << "x" << img.height() << " - " << img.depth() << "bpp" << std::endl;

    int bpp = (img.depth() == 32) ? GL_RGBA : GL_RGB;
    int format = (img.depth() == 32) ? GL_BGRA : GL_BGR;

    initGL(bpp, img.width(), img.height(), format, GL_UNSIGNED_BYTE, img.bits() );

}

Texture::Texture (unsigned char color[3]){
    initGL(GL_RGB, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, color);
}

void Texture::initGL(int bytesperpixel, int width, int height, int format, int type, unsigned char * data)
{
    mWidth=width;
    mHeight=height;
    mFormat = (bytesperpixel == GL_RGBA);
    mPixOrder = (GL_RGB == format);
    mPixels = new unsigned char[mWidth*mHeight*(3+mFormat)];
    memcpy(mPixels, data, mWidth*mHeight*(3+mFormat)*sizeof(unsigned char));

    glAssert(glGenTextures(1, &mTexId));
    glAssert(glBindTexture(GL_TEXTURE_2D, mTexId));
    glAssert(glTexImage2D(GL_TEXTURE_2D, 0, bytesperpixel, width, height, 0, format, type, data));
    glAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    glAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    glAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    glAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
}


void Texture::useMipMap(GLenum minFliter, GLenum magFilter)
{
    glAssert(glBindTexture(GL_TEXTURE_2D, mTexId));
    glAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter));
    glAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFliter));
    glAssert(glGenerateMipmap(GL_TEXTURE_2D));
}

void Texture::setFilter(GLenum minFliter, GLenum magFilter)
{
    glAssert(glBindTexture(GL_TEXTURE_2D, mTexId));
    glAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter));
    glAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFliter));
}
void Texture::setClamp(GLenum warpS, GLenum warpT)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, warpS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, warpT);
}


void Texture::bind(int unit)
{
    GLenum texUnit = GL_TEXTURE0+unit;
    glAssert(glActiveTexture(texUnit));
    glAssert(glBindTexture(GL_TEXTURE_2D, mTexId));
}

void Texture::deleteGL()
{
    glDeleteTextures(1, &mTexId);
}


void Texture::getPixel(float u, float v, glm::vec3 &color) {
    float scaledU = u * mWidth;
    float scaledV = v * mHeight;
    int s = (int)scaledU;
    int t = (int)scaledV;

    glm::vec3 centerColor;
    int linPos = (t*mWidth + s)*(3+mFormat);
    if (mPixOrder) {
        centerColor.x = ((float) mPixels[linPos])/255.0;
        centerColor.y = ((float) mPixels[linPos+1])/255.0;
        centerColor.z = ((float) mPixels[linPos+2])/255.0;
    } else {
        centerColor.x = ((float) mPixels[linPos+2])/255.0;
        centerColor.y = ((float) mPixels[linPos+1])/255.0;
        centerColor.z = ((float) mPixels[linPos])/255.0;
    }
    color = centerColor;

    // filtrage todo
    float ds = scaledU-s;
    float dt = scaledV-t;
    glm::vec3 rightColor;
    if (s<(mWidth-1)){
        int posds = (t*mWidth + (s+1))*(3+mFormat);
        if (mPixOrder) {
            rightColor.x = ((float) mPixels[posds])/255.0;
            rightColor.y = ((float) mPixels[posds+1])/255.0;
            rightColor.z = ((float) mPixels[posds+2])/255.0;
        } else {
            rightColor.x = ((float) mPixels[posds+2])/255.0;
            rightColor.y = ((float) mPixels[posds+1])/255.0;
            rightColor.z = ((float) mPixels[posds])/255.0;
        }

    } else {
        rightColor = centerColor;
    }

    glm::vec3 upColor;
    if (t<(mHeight-1)){
        int posdt = ((t+1)*mWidth + s)*(3+mFormat);
        if (mPixOrder) {
            upColor.x = ((float) mPixels[posdt])/255.0;
            upColor.y = ((float) mPixels[posdt+1])/255.0;
            upColor.z = ((float) mPixels[posdt+2])/255.0;
        } else {
            upColor.x = ((float) mPixels[posdt+2])/255.0;
            upColor.y = ((float) mPixels[posdt+1])/255.0;
            upColor.z = ((float) mPixels[posdt])/255.0;
        }

    } else {
        upColor = centerColor;
    }

    glm::vec3 upRightColor;
    if ( (t<(mHeight-1)) && (s<(mWidth-1)) ){
        int posdtds = ((t+1)*mWidth + s+1)*(3+mFormat);
        if (mPixOrder) {
            upRightColor.x = ((float) mPixels[posdtds])/255.0;
            upRightColor.y = ((float) mPixels[posdtds+1])/255.0;
            upRightColor.z = ((float) mPixels[posdtds+2])/255.0;
        } else {
            upRightColor.x = ((float) mPixels[posdtds+2])/255.0;
            upRightColor.y = ((float) mPixels[posdtds+1])/255.0;
            upRightColor.z = ((float) mPixels[posdtds])/255.0;
        }

    } else {
        upRightColor = centerColor;
    }




    rightColor = (1-ds)*centerColor + ds*rightColor;
    upColor = (1-ds)*upColor + ds*upRightColor;
    color = (1-dt)*rightColor + dt*upColor;

}

}
