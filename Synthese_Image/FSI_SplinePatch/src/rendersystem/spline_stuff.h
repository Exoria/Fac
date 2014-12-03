#ifndef SPLINE_STUFF
#define SPLINE_STUFF

#include <vector>
#include "glm/glm.hpp"
#include "opengl.h"
#include "parametricmeshes.h"

namespace rendersystem{

enum KnotVectorType {OpenUniform, Uniform};


//==============================================================================================================================

//-------------------------------------------
// Class BSpline3D
//-------------------------------------------
class BSpline3D {
public:
    // Constructor
    BSpline3D(std::vector< glm::vec3 > pts, int order, KnotVectorType type, int substeps, const glm::mat4 transform);

    // Curve evaluation method
    glm::vec3 eval(float u) const;

    // OpenGl related methods
    void compileGL();
    void destroyGL();
    void drawGL(const glm::mat4x4 &sceneToViewMatrix, const glm::mat4x4 &projectionMatrix, int transformLocation[4], int colorLocation);

    // Curve related attributes
    std::vector< glm::vec3 > mCtrl_pts;
    std::vector< float > mKnots;
    int mOrder;

private :
    // OpenGL related attributes
    glm::mat4 mModelToSceneMatrix;
    GLuint mVertexArrayObject;
    enum {VBO_VERTICES, VBO_INDICES};
    GLuint mVertexBufferObjects[2];
    std::vector<glm::vec3> mMesh;
    std::vector<int> mIndices;
    int mSubdivSteps;
};


//==============================================================================================================================

//-------------------------------------------
// Class BSplinePatch
//-------------------------------------------
class BSplinePatch{
public:
    // Constructor
    BSplinePatch( int orderU, int orderV, int knotVectorType, const std::vector<glm::vec3> &points, int curveSizeU, int curveSizeV, int substepsU, int substepsV, const glm::mat4 transform);

    // Patch evaluation method
    glm::vec3 evalSurface(float u, float v) const;
    glm::vec3 evalSurfaceNormal(float u, float v) const;
    std::vector<glm::vec3> directricePointsU(int baseIndex) const;
    std::vector<glm::vec3> directricePointsV(int baseIndex) const;

    // OpenGl related methods
    void compileGL();
    void destroyGL();
    void drawGL(const glm::mat4x4 &sceneToViewMatrix, const glm::mat4x4 &projectionMatrix, int transformLocation[4], int colorLocation);

    // Patch related attributes
    std::vector<glm::vec3> mCtrl_pts;
    std::vector<float> mKnotsU;
    std::vector<float> mKnotsV;
    int mOrderU;
    int mOrderV;
    int mCurveSizeU;
    int mCurveSizeV;

private:
    // OpenGl related attributes
    glm::mat4 mModelToSceneMatrix;
    GLuint mVertexArrayObject;
    enum {VBO_VERTICES, VBO_INDICES};
    GLuint mVertexBufferObjects[2];
    std::vector<glm::vec3> mMesh;
    std::vector<int> mIndices;
    int mSubdivStepsU;
    int mSubdivStepsV;
};


//==============================================================================================================================

//-------------------------------------------
// Class ParametricBSplinePatch
//-------------------------------------------
class ParametricBSplinePatch : public ParametricMesh {
public:
    // Constructor
    ParametricBSplinePatch( BSplinePatch *patch ) : ParametricMesh(), mPatch(patch){}

protected:
    // Surface evaluation method
    Vertex evalSurface(float u, float v) const;

    // Patch
    BSplinePatch *mPatch;
};

} // end rendersystem

#endif
