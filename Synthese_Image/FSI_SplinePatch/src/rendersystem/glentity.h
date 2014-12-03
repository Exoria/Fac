#ifndef GLENTITY_H
#define GLENTITY_H

#include "opengl.h"
#include "glmesh.h"
#include "glmaterial.h"

namespace rendersystem {

//-------------------------------------------
// Classe MyGLEntity
//-------------------------------------------
class MyGlEntity {
    MyGLMesh *mTheMesh;
    MyGLMaterial *mTheMaterial;
    glm::mat4 mModelToSceneMatrix;

public:
    MyGlEntity(MyGLMesh *theMesh, MyGLMaterial *theMaterial, const glm::mat4 &theTransformation) : mTheMesh(theMesh), mTheMaterial(theMaterial), mModelToSceneMatrix(theTransformation) {}

    void drawGL(const glm::mat4x4 &sceneToViewMatrix, const glm::mat4x4 &projectionMatrix, int transformLocation[4], int materialLocation[4]);

    glm::mat4 getModelMatrix() const {return mModelToSceneMatrix;}

    MyGLMesh * getMesh() const {return mTheMesh;}

    MyGLMaterial *getMaterial() const {return mTheMaterial;}
};


}
#endif // GLENTITY_H
