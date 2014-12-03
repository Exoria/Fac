#include "spline_stuff.h"

namespace rendersystem{


//-------------------------------------------
// Class BSplineCurveEvaluator
//-------------------------------------------
class BSplineCurveEvaluator {
public :
    static glm::vec3 evaluate (const std::vector<glm::vec3> &points,
                               const std::vector<float> &knots,
                               int order,
                               float u){
        glm::vec3 ret;
        //TODO
        //@BEGIN
        // determine the value to use for u according to knots
        // determine the [u_n, u_n1] interval that contains u
        // prepare data for blossom (pts + knots)
        // return result of blossom
        //@END
        return ret;
    }

    static glm::vec3 evaluateTan (const std::vector<glm::vec3> &points,
                                  const std::vector<float> &knots,
                                  int order,
                                  float u){
        glm::vec3 ret;
        //TODO
        //@BEGIN
        // determine the value to use for u according to knots
        // determine the [u_n, u_n1] interval that contains u
        // compute delta_pts
        // prepare data for blossom (delta_pts + knots)
        // return result of blossom
        //@END
        return ret;
    }

    static glm::vec3 blossom(std::vector<glm::vec3> &points,
                             std::vector<float>   &knots,
                             int order,
                             float u){
        glm::vec3 ret;
        //TODO
        //@BEGIN
        //@END
        return ret;
    }
};


//==============================================================================================================================

//-------------------------------------------
// Classe BSpline3D
//-------------------------------------------

BSpline3D::BSpline3D(std::vector< glm::vec3 > pts, int order, KnotVectorType type, int substeps, const glm::mat4 transform)
    : mCtrl_pts(pts), mOrder(order), mModelToSceneMatrix(transform), mSubdivSteps(substeps)
{
    //TODO
    //@BEGIN
    // build knot vector
    //@END

    // record ctrl pts
    mMesh = mCtrl_pts;
    // record curve pts
    for (int i=0; i<=mSubdivSteps; ++i){
        mMesh.push_back( eval( (float)i/mSubdivSteps ) );
    }
    // record ctrl pts idx for ctrl polygon draw
    for(unsigned int i = 0; i<mCtrl_pts.size(); ++i ){
        mIndices.push_back(i);
    }
    // record curve pts idx for curve draw
    int offset = mCtrl_pts.size();
    for(int j=0; j<=mSubdivSteps; ++j) {
        mIndices.push_back(offset+j);
    }

}

glm::vec3 BSpline3D::eval(float u) const{
    glm::vec3 ret;
    return BSplineCurveEvaluator::evaluate(mCtrl_pts, mKnots, mOrder, u);
    return ret;
}

void BSpline3D::compileGL(){
    glAssert( glGenVertexArrays(1, &mVertexArrayObject) );
    glAssert( glGenBuffers(2, mVertexBufferObjects) );
    glAssert( glBindVertexArray(mVertexArrayObject) );
    glAssert( glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObjects[VBO_VERTICES]) );

    glAssert( glBufferData(GL_ARRAY_BUFFER, mMesh.size() * sizeof(glm::vec3),  &(mMesh[0]), GL_STATIC_DRAW) );
    GLuint stride = sizeof(glm::vec3);
    GLboolean normalized = GL_FALSE;
    GLenum type = GL_FLOAT;
    GLuint index = 0;
    GLint size = 3;
    GLvoid *pointer = BUFFER_OFFSET(0);
    glAssert( glVertexAttribPointer(index, size, type, normalized, stride, pointer) );
    glAssert( glEnableVertexAttribArray(index) );

    glAssert( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVertexBufferObjects[VBO_INDICES]) );
    glAssert( glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(int),  &(mIndices[0]), GL_STATIC_DRAW) );
    glAssert( glBindVertexArray(0) );
}

void BSpline3D::destroyGL(){
    glAssert(glDeleteVertexArrays(1, &mVertexArrayObject));
    mVertexArrayObject = 0;
    glAssert(glDeleteBuffers(2, mVertexBufferObjects));
    mVertexBufferObjects[0] = 0;
    mVertexBufferObjects[1] = 1;
}

void BSpline3D::drawGL(const glm::mat4x4 &sceneToViewMatrix, const glm::mat4x4 &projectionMatrix, int transformLocation[4], int colorLocation){
    glm::mat4x4 localprojectionMatrix = projectionMatrix;
    glm::mat4x4 localmodelViewMatrix = sceneToViewMatrix*mModelToSceneMatrix;
    glm::mat4x4 normalMatrix = glm::transpose(glm::inverse(localmodelViewMatrix));
    glm::mat4x4 MVP = localprojectionMatrix * localmodelViewMatrix;
    glAssert( glUniformMatrix4fv( transformLocation[0], 1, GL_FALSE, glm::value_ptr(localmodelViewMatrix)) );
    glAssert( glUniformMatrix4fv( transformLocation[1], 1, GL_FALSE, glm::value_ptr(localprojectionMatrix)));
    glAssert( glUniformMatrix4fv( transformLocation[2], 1, GL_FALSE, glm::value_ptr(normalMatrix)));
    glAssert( glUniformMatrix4fv( transformLocation[3], 1, GL_FALSE, glm::value_ptr(MVP)));

    glAssert( glBindVertexArray(mVertexArrayObject) );
    glAssert( glEnable(GL_BLEND) );
    glAssert( glBlendFunc(GL_SRC_ALPHA, GL_ONE) );
    glAssert( glEnable(GL_LINE_SMOOTH) );

    // draw ctrl polygon
    glm::vec4 color = glm::vec4(1,1,1,.5);
    glAssert( glLineWidth(1.5f) );
    glAssert( glUniform4fv( colorLocation, 1, glm::value_ptr(color)) );
    glAssert( glDrawElements(GL_LINE_STRIP, mCtrl_pts.size(), GL_UNSIGNED_INT, NULL) );
    glAssert( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );
    // draw curve
    color = glm::vec4(1,0,0,1);
    glAssert( glLineWidth(3.f) );
    glAssert( glUniform4fv( colorLocation, 1, glm::value_ptr(color)) );
    int offset = mCtrl_pts.size();
    glAssert( glDrawElements(GL_LINE_STRIP, mSubdivSteps+1, GL_UNSIGNED_INT, BUFFER_OFFSET(offset*sizeof(int))) );

    glAssert( glLineWidth(1.f) );
    glAssert( glDisable(GL_BLEND) );
}


//==============================================================================================================================

//-------------------------------------------
// Class ParametricBSplinePatch
//-------------------------------------------
BSplinePatch::BSplinePatch( int orderU,
                            int orderV,
                            int knotVectorType,
                            const std::vector<glm::vec3> &points,
                            int curveSizeU,
                            int curveSizeV,
                            int substepsU,
                            int substepsV,
                            const glm::mat4 transform)
    :  mCtrl_pts(points), mOrderU(orderU), mOrderV(orderV),  mCurveSizeU(curveSizeU),  mCurveSizeV(curveSizeV), mModelToSceneMatrix(transform), mSubdivStepsU(substepsU), mSubdivStepsV(substepsV)
{
    //TODO
    //@BEGIN
    // build knot vector
    //@END

    // register control points
    mMesh = mCtrl_pts;

    // register control curves points
    for(int j=0; j<mCurveSizeV; ++j) {
        std::vector<glm::vec3> dir = directricePointsU(j);
        for(int i=0; i<=mSubdivStepsU; ++i){
            float u = float(i)/float(mSubdivStepsU);
            mMesh.push_back( BSplineCurveEvaluator::evaluate(dir, mKnotsU, mOrderU, u) );
        }
    }
    for(int i=0; i<mCurveSizeU; ++i){
        std::vector<glm::vec3> dir = directricePointsV(i);
        for(int j=0; j<=mSubdivStepsV; ++j) {
            float v = float(j)/float(mSubdivStepsV);
            mMesh.push_back( BSplineCurveEvaluator::evaluate(dir, mKnotsV, mOrderV, v) );
        }
    }

    // register ctrl points indices for ctrl polygons draw
    for(int i = 0; i<mCurveSizeV; ++i ){
        for(int j = 0; j<mCurveSizeU-1; ++j ){
            mIndices.push_back(i*mCurveSizeU + j);
            mIndices.push_back(i*mCurveSizeU + j+1);
        }
    }
    for(int j = 0; j<mCurveSizeU; ++j ){
        for(int i = 0; i<mCurveSizeV-1; ++i ){
            mIndices.push_back(i*mCurveSizeU + j);
            mIndices.push_back((i+1)*mCurveSizeU + j);
        }
    }

    // register ctrl curves points indices for ctrl curves draw
    int offset = mCurveSizeU*mCurveSizeV;
    for(int i=0; i<mCurveSizeV; ++i){
        for(int j=0; j<=mSubdivStepsU; ++j) {
            mIndices.push_back(offset + i*(mSubdivStepsU+1) + j);
        }
    }
    offset += mCurveSizeV*(mSubdivStepsU+1);
    for(int j=0; j<mCurveSizeU; ++j) {
        for(int i=0; i<=mSubdivStepsV; ++i){
            mIndices.push_back(offset + j*(mSubdivStepsV+1) + i);
        }
    }
}

std::vector<glm::vec3> BSplinePatch::directricePointsU(int baseIndex) const{
    std::vector<glm::vec3> ret;
    //TODO
    //@BEGIN
    // get points of the U directrice curve of index baseIndex
    //@END
    return ret;
}

std::vector<glm::vec3> BSplinePatch::directricePointsV(int baseIndex) const{
    std::vector<glm::vec3> ret;
    //TODO
    //@BEGIN
    // get points of the V directrice curve of index baseIndex
    //@END
    return ret;
}

glm::vec3 BSplinePatch::evalSurface(float u, float v) const {
    glm::vec3 ret;
    //TODO
    //@BEGIN
    // compute V directrice
    // compute surface point
    //@END
    return ret;
}

glm::vec3 BSplinePatch::evalSurfaceNormal(float u, float v) const {
    glm::vec3 ret(1, 0, 0);
    //TODO
    //@BEGIN
    // compute V directrice
    // compute V tangent
    // compute U directrice
    // compute U tangent
    // compute normal
    //@END
    return ret;
}

void BSplinePatch::compileGL() {
    glAssert( glGenVertexArrays(1, &mVertexArrayObject) );
    glAssert( glGenBuffers(2, mVertexBufferObjects) );
    glAssert( glBindVertexArray(mVertexArrayObject) );
    glAssert( glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObjects[VBO_VERTICES]) );


    glAssert( glBufferData(GL_ARRAY_BUFFER, mMesh.size() * sizeof(glm::vec3),  &(mMesh[0]), GL_STATIC_DRAW) );
    GLuint stride = sizeof(glm::vec3);
    GLboolean normalized = GL_FALSE;
    GLenum type = GL_FLOAT;

    GLuint index = 0;
    GLint size = 3;
    GLvoid *pointer = BUFFER_OFFSET(0);
    glAssert( glVertexAttribPointer(index, size, type, normalized, stride, pointer) );
    glAssert( glEnableVertexAttribArray(index) );

    glAssert( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVertexBufferObjects[VBO_INDICES]) );
    glAssert( glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(int),  &(mIndices[0]), GL_STATIC_DRAW) );
    glAssert( glBindVertexArray(0) );

}

void BSplinePatch::destroyGL(){
    glAssert(glDeleteVertexArrays(1, &mVertexArrayObject));
    mVertexArrayObject = 0;
    glAssert(glDeleteBuffers(2, mVertexBufferObjects));
    mVertexBufferObjects[0] = 0;
    mVertexBufferObjects[1] = 1;
}


void BSplinePatch::drawGL(const glm::mat4x4 &sceneToViewMatrix, const glm::mat4x4 &projectionMatrix, int transformLocation[4], int colorLocation){
    glm::mat4x4 localprojectionMatrix = projectionMatrix;
    glm::mat4x4 localmodelViewMatrix = sceneToViewMatrix*mModelToSceneMatrix;
    glm::mat4x4 normalMatrix = glm::transpose(glm::inverse(localmodelViewMatrix));
    glm::mat4x4 MVP = localprojectionMatrix * localmodelViewMatrix;
    glAssert( glUniformMatrix4fv( transformLocation[0], 1, GL_FALSE, glm::value_ptr(localmodelViewMatrix)) );
    glAssert( glUniformMatrix4fv( transformLocation[1], 1, GL_FALSE, glm::value_ptr(localprojectionMatrix)));
    glAssert( glUniformMatrix4fv( transformLocation[2], 1, GL_FALSE, glm::value_ptr(normalMatrix)));
    glAssert( glUniformMatrix4fv( transformLocation[3], 1, GL_FALSE, glm::value_ptr(MVP)));

    glAssert( glBindVertexArray(mVertexArrayObject) );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_LINE_SMOOTH);

    // draw ctrl polygon
    glm::vec4 color = glm::vec4(1,1,1,.5);
    glAssert( glLineWidth(1.5f) );
    glAssert( glUniform4fv( colorLocation, 1, glm::value_ptr(color)) );
    glAssert( glDrawElements(GL_LINES, 2*mCurveSizeV*(mCurveSizeU-1) + 2*mCurveSizeU*(mCurveSizeV-1), GL_UNSIGNED_INT, NULL) );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // draw ctrl curves U
    color = glm::vec4(1,0,0,1);
    glAssert( glLineWidth(3.f) );
    glAssert( glUniform4fv( colorLocation, 1, glm::value_ptr(color)) );
    int offset = 2*mCurveSizeV*(mCurveSizeU-1) + 2*mCurveSizeU*(mCurveSizeV-1);
    for(int i = 0; i<mCurveSizeV; ++i){
        glAssert( glDrawElements(GL_LINE_STRIP, mSubdivStepsU+1, GL_UNSIGNED_INT, BUFFER_OFFSET(offset*sizeof(int))) );
        offset += mSubdivStepsU+1;
    }

    // draw ctrl curves V
    color = glm::vec4(0,0,1,1);
    glAssert( glUniform4fv( colorLocation, 1, glm::value_ptr(color)) );
    for(int i = 0; i<mCurveSizeU; ++i){
        glAssert( glDrawElements(GL_LINE_STRIP, mSubdivStepsV+1, GL_UNSIGNED_INT, BUFFER_OFFSET(offset*sizeof(int))) );
        offset += mSubdivStepsV+1;
    }

    glLineWidth(1.f);

    glDisable(GL_BLEND);
}


//==============================================================================================================================

//-------------------------------------------
// Class ParametricBSplinePatch
//-------------------------------------------
loaders::Mesh::Vertex ParametricBSplinePatch::evalSurface(float u, float v) const {
    Vertex vert = ParametricMesh::evalSurface(u,v);
    //TODO
    //@BEGIN
    // compute position and normal
    //@END
    return vert;
}

}
