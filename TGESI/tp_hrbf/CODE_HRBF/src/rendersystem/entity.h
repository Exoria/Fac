#ifndef ENTITY_H
#define ENTITY_H

#include "material.h"

// =============================================================================
namespace rendersystem {
// =============================================================================

class Entity {
protected:
    MyGLMaterial* mTheMaterial;
    glm::mat4     mModelToSceneMatrix;
public:
    Entity(MyGLMaterial* theMaterial,
           const glm::mat4& theTransformation) :
        mTheMaterial(theMaterial),
        mModelToSceneMatrix(theTransformation)
    {  }

    virtual void drawGL(const glm::mat4x4 &sceneToViewMatrix,
                        const glm::mat4x4 &projectionMatrix,
                        int transformLocation[4],
                        int materialLocation[3])
    { }
};


}// END NAMESPACE rendersystem =================================================

#endif // ENTITY_H
