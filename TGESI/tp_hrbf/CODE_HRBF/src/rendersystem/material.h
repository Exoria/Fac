#ifndef MATERIAL_H
#define MATERIAL_H

#include "opengl.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>

// =============================================================================
namespace rendersystem {
// =============================================================================


class MyGLMaterial {
    glm::vec3 mKd;
    glm::vec3 mKs;
    float mNs;

public:
    MyGLMaterial(glm::vec3 kd, glm::vec3 ks, float n) : mKd(kd), mKs(ks), mNs(n) {}

    void bindGL(int parameterLocation[3]){
        glAssert( glUniform3fv( parameterLocation[0], 1, glm::value_ptr(mKd)) );
        glAssert( glUniform3fv( parameterLocation[1], 1, glm::value_ptr(mKs)) );
        glAssert( glUniform1f ( parameterLocation[2], mNs) );
    }
};


}// END NAMESPACE rendersystem =================================================

#endif // MATERIAL_H
