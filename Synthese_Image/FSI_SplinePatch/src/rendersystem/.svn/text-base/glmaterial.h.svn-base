#ifndef GLMATERIAL_H
#define GLMATERIAL_H

#include "opengl.h"
#include "texture.h"

namespace rendersystem {

//-------------------------------------------
// Classe Material
//-------------------------------------------
class MyGLMaterial {
    glm::vec3 mKd;
    glm::vec3 mKs;
    float mNs;

    Texture *mTexture;

public:
    MyGLMaterial(glm::vec3 kd, glm::vec3 ks, float n) : mKd(kd), mKs(ks), mNs(n), mTexture(NULL) {}

    void getParameters(glm::vec3 &kd, glm::vec3 &ks, float &n, Texture *&texture) {
        kd = mKd;
        ks = mKs;
        n=mNs;
        texture=mTexture;
    }

    void setTexture(Texture *tex);

    void bindGL(int parameterLocation[4]);
};


}
#endif // GLMATERIAL_H
