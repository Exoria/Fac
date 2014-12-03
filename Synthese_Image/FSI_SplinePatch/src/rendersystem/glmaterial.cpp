
#include "glmaterial.h"

namespace rendersystem {

//-------------------------------------------
// Classe Material
//-------------------------------------------

    void MyGLMaterial::setTexture(Texture *tex){
        mTexture = tex;
    }

    void MyGLMaterial::bindGL(int parameterLocation[4]){
        glAssert( glUniform3fv( parameterLocation[0], 1, glm::value_ptr(mKd)) );
        glAssert( glUniform3fv( parameterLocation[1], 1, glm::value_ptr(mKs)) );
        glAssert( glUniform1f( parameterLocation[2], mNs) );
        mTexture->bind(0);
        glAssert( glUniform1i(parameterLocation[3], 0) );
    }


}
