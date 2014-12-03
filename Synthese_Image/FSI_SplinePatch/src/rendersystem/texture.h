#ifndef TEXTURE_H
#define TEXTURE_H
#include "opengl.h"

#include <QImage>

#include <string>

namespace rendersystem {

class Texture {
    GLuint mTexId;

public :
    Texture (std::string filename);

    Texture (unsigned char color[3]);

    void initGL(int bytesperpixel, int width, int height, int format, int type, unsigned char * data);

    void useMipMap(GLenum minFliter, GLenum magFilter);

    void setFilter(GLenum minFliter, GLenum magFilter);

    void setClamp(GLenum warpS, GLenum warpT);

    void bind(int unit);

    void deleteGL();


    // public pour le moment
    // pixels
    unsigned char *mPixels;
    int mFormat;
    int mPixOrder;
    int mWidth;
    int mHeight;

    // u and v between 0 and 1
    void getPixel(float u, float v, glm::vec3 &color);
};


}

#endif // TEXTURE_H
