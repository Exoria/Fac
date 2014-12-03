/***************************************************************************
 *   Copyright (C) 2012 by Mathias Paulin                                  *
 *   Mathias.Paulin@irit.fr                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "renderer.h"

#include "objloader.h"
#include "fileloader.h"
#include "icosahedron.h"
#include "my_glmesh.h"
#include "parametric_mesh.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>

#include <Eigen/Dense>
#include "gldirect_draw.h"
#include "hrbf/hrbf_phi_funcs.h"
#include "hrbf/hrbf_core.h"
#include "mcubes/iso_extractor_mcube.h"
#include "hrbf/bbox.h"

GlDirect_draw* display_list;
rendersystem::MyGLMesh* make_hrbf(int res);

// =============================================================================
namespace rendersystem {
// =============================================================================


//-------------------------------------------
/** @defgroup RendererGlobalFunctions
  * @author Mathias Paulin <Mathias.Paulin@irit.fr>
  * @ingroup RenderSystem
  * Global functions for the renderer.
  */
//-------------------------------------------

/**
  * @ingroup RendererGlobalFunctions
  * Print to stderr compilation log of a shader
  */
void printShaderInfoLog(GLint shader)
{
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;
    glAssert(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen));
    if (infoLogLen > 0) {
        infoLog = new GLchar[infoLogLen];
        // error check for fail to allocate memory omitted
        glAssert(glGetShaderInfoLog(shader, infoLogLen, &charsWritten, infoLog));
        std::cerr << "InfoLog:" << std::endl << infoLog << std::endl;
        delete [] infoLog;
    }
}

/**
  * @ingroup RendererGlobalFunctions
  * Print to stderr link log of a program
  */
void printProgramInfoLog(GLint program)
{
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;
    glAssert(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen));
    if (infoLogLen > 0) {
        infoLog = new GLchar[infoLogLen];
        // error check for fail to allocate memory omitted
        glAssert(glGetProgramInfoLog(program, infoLogLen, &charsWritten, infoLog));
        std::cerr << "InfoLog:" << std::endl << infoLog << std::endl;
        delete [] infoLog;
    }
}

// -----------------------------------------------------------------------------
// Entities
// -----------------------------------------------------------------------------
class MyGLEntity : public Entity {
    MyGLMesh* mTheMesh;
public:
    MyGLEntity(MyGLMesh* theMesh,
               MyGLMaterial* theMaterial,
               const glm::mat4& theTransformation) :
        mTheMesh(theMesh),
        Entity(theMaterial, theTransformation)
    {  }

    void drawGL(const glm::mat4x4 &sceneToViewMatrix,
                const glm::mat4x4 &projectionMatrix,
                int transformLocation[4],
                int materialLocation[3])
    {
        glm::mat4x4 localprojectionMatrix = projectionMatrix;
        glm::mat4x4 localmodelViewMatrix = sceneToViewMatrix*mModelToSceneMatrix;
        glm::mat4x4 normalMatrix = glm::transpose(glm::inverse(localmodelViewMatrix));
        glm::mat4x4 MVP = localprojectionMatrix * localmodelViewMatrix;

        glAssert( glUniformMatrix4fv( transformLocation[0], 1, GL_FALSE, glm::value_ptr(localmodelViewMatrix)) );
        glAssert( glUniformMatrix4fv( transformLocation[1] , 1, GL_FALSE, glm::value_ptr(localprojectionMatrix)));
        glAssert( glUniformMatrix4fv( transformLocation[2] , 1, GL_FALSE, glm::value_ptr(normalMatrix)));
        glAssert( glUniformMatrix4fv( transformLocation[3] , 1, GL_FALSE, glm::value_ptr(MVP)));

        mTheMaterial->bindGL(materialLocation);
        mTheMesh->drawGL();
    }
};

// -----------------------------------------------------------------------------

class DispListEntity : public Entity {
    GlDirect_draw* mList;
public:
    DispListEntity(GlDirect_draw* list,
                   MyGLMaterial* theMaterial,
                   const glm::mat4& theTransformation) :
        mList(list),
        Entity(theMaterial, theTransformation)
    {  }

    void drawGL(const glm::mat4x4 &sceneToViewMatrix,
                const glm::mat4x4 &projectionMatrix,
                int transformLocation[4],
                int materialLocation[3])
    {
        glm::mat4x4 localprojectionMatrix = projectionMatrix;
        glm::mat4x4 localmodelViewMatrix  = sceneToViewMatrix*mModelToSceneMatrix;

        glPointSize(5.f);
        display_list->set_matrix( glm::value_ptr(localmodelViewMatrix),
                                  glm::value_ptr(localprojectionMatrix) );
        display_list->draw();
    }

};


// -----------------------------------------------------------------------------
// Méthodes de Renderer
// -----------------------------------------------------------------------------
void Renderer::initRessources(){
    glAssert( glEnable(GL_DEPTH_TEST) );
    glAssert( glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) );
    initView();
    initGeometry();
    initShaders();
}

// -----------------------------------------------------------------------------

void Renderer::initView(){
    glm::vec3 eye(1.f, 0.25f, 1.f);
    glm::vec3 center(0.0f, 0.0f, 0.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    g_viewMatrix = glm::lookAt(eye,center,up);
}

// -----------------------------------------------------------------------------

void Renderer::initGeometry(){

    std::vector<loaders::Mesh*> objects;
#if 0
    loaders::obj_mtl::ObjLoader loader;
    QString reason;
    if (!loader.load("../data/camel.obj",reason))
        std::cerr << "File not loaded. Reason : " << std::endl << reason.toStdString() << std::endl;
    else
        loader.getObjects(objects);
#endif

    MyGLMaterial *defaultMaterial = new MyGLMaterial(glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), 25.f);

    for (std::vector<loaders::Mesh*>::iterator it = objects.begin(); it != objects.end(); ++it ) {
        MyGLMesh *newObject = new MyGLMesh(*(*it));
        g_meshes.push_back( newObject );
        glm::mat4 theTransformation(1.0f);
        MyGLEntity * newEntity = new MyGLEntity(newObject, defaultMaterial, theTransformation);
        //        g_entities.push_back(newEntity);      ------------------------------------------------------DEBUG
        delete (*it);
    }

    ParametricMesh* plane       = new ParametricMesh();
    ParametricMesh* sphere      = new ParametricSphere();
    ParametricMesh* cylinder    = new ParametricCylindre();
    ParametricMesh* cone        = new ParametricCone();
    Icosahedron*    icosahedron = new Icosahedron();
    IcoSphere*      icosphere   = new IcoSphere(*icosahedron, 6);

    plane->   generateMesh(25,  50 );
    sphere->  generateMesh(50,  25 );
    cylinder->generateMesh(100, 100);
    cone->    generateMesh(100, 100);

    MyGLMesh *glIcosahedre = new MyGLMesh(*icosahedron);
    MyGLMesh *glIcoSphere  = new MyGLMesh(*icosphere);
    MyGLMesh *glPlane      = new MyGLMesh(*plane);
    MyGLMesh *glSphere     = new MyGLMesh(*sphere);
    MyGLMesh *glCylinder   = new MyGLMesh(*cylinder);
    MyGLMesh *glCone       = new MyGLMesh(*cone);

    g_meshes.push_back( glPlane );
    g_meshes.push_back( glSphere );
    g_meshes.push_back( glCylinder );
    g_meshes.push_back( glCone );
    g_meshes.push_back( glIcosahedre );
    g_meshes.push_back( glIcoSphere );
    delete plane;
    delete sphere;
    delete cylinder;
    delete cone;
    delete icosahedron;
    delete icosphere;

    // Un plan
    glm::mat4 theTransformation(1.0f);
    {
        theTransformation = glm::rotate(theTransformation, -90.0f, glm::vec3(1.f,0.f,0.f));
        theTransformation = glm::scale(theTransformation, glm::vec3(10.f,10.f,1.f));
        theTransformation = glm::translate(theTransformation, glm::vec3(0.f,0.f,-0.490f));

        MyGLEntity * newEntity = new MyGLEntity(glPlane, defaultMaterial, theTransformation);
//        g_entities.push_back(newEntity);
    }

    MyGLMaterial* hrbfMat = new MyGLMaterial(glm::vec3(1.f, 1.f, 0.01f), glm::vec3(1.0f, 1.0f, 1.0f), 25.f);
    glm::mat4 hrbfTransfo = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f,0.1f,0.1f));
    display_list = new GlDirect_draw();
    g_entities.push_back(new DispListEntity(display_list, hrbfMat, hrbfTransfo) );

//    display_list->begin(GL_TRIANGLES);
//    display_list->vertex3f(0,0,0);
//    display_list->vertex3f(0,1,0);
//    display_list->vertex3f(0,0,1);
//    display_list->end();
//    display_list->enable_internal_shader(false);
//    display_list->set_attr_index(GlDirect_draw::ATTR_POSITION, 0);
//    display_list->set_attr_index(GlDirect_draw::ATTR_NORMAL  , 1);

    // HRBF ===========
    MyGLMesh* hrbf = make_hrbf(16);
    g_meshes.push_back( hrbf );
    MyGLEntity* hrbfEntity = new MyGLEntity(hrbf, hrbfMat, hrbfTransfo);
    g_entities.push_back(hrbfEntity);

    // Construction du repère

    theTransformation = glm::scale(glm::mat4(1.0f), glm::vec3(1.f,1.f,1.f));

    // La sphère pour noter l'Origine
    MyGLMaterial *centerMaterial = new MyGLMaterial(glm::vec3(1.f, 1.f, 0.01f), glm::vec3(1.0f, 1.0f, 1.0f), 25.f);
    glm::mat4 sphereTransformation = glm::scale(theTransformation, glm::vec3(0.5f,0.5f,0.5f));
    MyGLEntity * newEntity = new MyGLEntity(glIcoSphere, centerMaterial, sphereTransformation);
//    g_entities.push_back(newEntity);

    // axe Z
    {
        MyGLMaterial *zAxisMaterial = new MyGLMaterial(glm::vec3(0.01f, 0.01f, 1.0f), glm::vec3(1.f, 1.f, 1.f), 50.f);

        glm::mat4 cylinderTransformation = glm::scale(theTransformation, glm::vec3(0.03f, 0.03f, 0.9f));
        newEntity = new MyGLEntity(glCylinder, zAxisMaterial, cylinderTransformation);
        //        g_entities.push_back(newEntity);------------------------------------------------------DEBUG

        glm::mat4 coneTransformation = glm::rotate(theTransformation, 180.0f, glm::vec3(0.f, 1.f, 0.f));
        coneTransformation = glm::scale(coneTransformation, glm::vec3(0.06f,0.06f,0.2f));
        coneTransformation = glm::translate(coneTransformation, glm::vec3(0.f,0.f,-5.f));

        newEntity = new MyGLEntity(glCone, zAxisMaterial, coneTransformation);
        //        g_entities.push_back(newEntity);------------------------------------------------------DEBUG
    }

    // axe X
    theTransformation = glm::rotate(theTransformation, 90.0f, glm::vec3(0.f, 1.f, 0.f));
    {
        MyGLMaterial *xAxisMaterial = new MyGLMaterial(glm::vec3(1.0f, 0.01f, 0.01f), glm::vec3(1.f, 1.f, 1.f), 50.f);

        glm::mat4 cylinderTransformation = glm::scale(theTransformation, glm::vec3(0.03f, 0.03f, 0.9f));
        newEntity = new MyGLEntity(glCylinder, xAxisMaterial, cylinderTransformation);
        //        g_entities.push_back(newEntity);------------------------------------------------------DEBUG

        glm::mat4 coneTransformation = glm::rotate(theTransformation, 180.0f, glm::vec3(0.f, 1.f, 0.f));
        coneTransformation = glm::scale(coneTransformation, glm::vec3(0.06f,0.06f,0.2f));
        coneTransformation = glm::translate(coneTransformation, glm::vec3(0.f,0.f,-5.f));

        newEntity = new MyGLEntity(glCone, xAxisMaterial, coneTransformation);
        //        g_entities.push_back(newEntity);------------------------------------------------------DEBUG
    }

    // axe Y
    theTransformation = glm::rotate(theTransformation, -90.0f, glm::vec3(1.f, 0.f, 0.f));
    {
        MyGLMaterial *yAxisMaterial = new MyGLMaterial(glm::vec3(0.01f, 1.f, 0.01f), glm::vec3(1.f, 1.f, 1.f), 50.f);

        glm::mat4 cylinderTransformation = glm::scale(theTransformation, glm::vec3(0.03f, 0.03f, 0.9f));
        newEntity = new MyGLEntity(glCylinder, yAxisMaterial, cylinderTransformation);
        //        g_entities.push_back(newEntity);------------------------------------------------------DEBUG

        glm::mat4 coneTransformation = glm::rotate(theTransformation, 180.0f, glm::vec3(0.f, 1.f, 0.f));
        coneTransformation = glm::scale(coneTransformation, glm::vec3(0.06f,0.06f,0.2f));
        coneTransformation = glm::translate(coneTransformation, glm::vec3(0.f,0.f,-5.f));

        newEntity = new MyGLEntity(glCone, yAxisMaterial, coneTransformation);
        //        g_entities.push_back(newEntity);------------------------------------------------------DEBUG
    }


    // 3 - Compiler les maillage pour préparer leur affichage en openGL.
    for (std::vector<MyGLMesh*>::iterator it = g_meshes.begin(); it != g_meshes.end(); ++it ) {
        (*it)->compileGL();
    }

    // Fin du code à écrire
}

// -----------------------------------------------------------------------------

void Renderer::initShaders(){
    char* vertexShaderSource = loaders::text::loadFile("../shaders/vertexdefault.glsl");
    int vertexShaderId;
    glAssert(vertexShaderId = glCreateShader(GL_VERTEX_SHADER));
    glAssert( glShaderSource(vertexShaderId, 1, (const GLchar **)&vertexShaderSource, NULL) );
    glAssert(glCompileShader(vertexShaderId));
    GLint compiled;
    glAssert(glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &compiled));
    if (!compiled) {
        std::cerr << " Vertex shader not compiled : " << std::endl;
        printShaderInfoLog(vertexShaderId);
    }

    char *fragmentShaderSource;
    fragmentShaderSource = loaders::text::loadFile("../shaders/fragmentdefault.glsl");
    int fragmentShaderId;
    glAssert(fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER));
    glAssert( glShaderSource(fragmentShaderId, 1, (const GLchar **)&fragmentShaderSource, NULL) );
    glAssert(glCompileShader(fragmentShaderId));
    glAssert(glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &compiled));
    if (!compiled) {
        std::cerr << " fragment shader not compiled : " << std::endl;
        printShaderInfoLog(fragmentShaderId);
    }

    glAssert(g_program = glCreateProgram());
    glAssert( glAttachShader(g_program, vertexShaderId) );
    glAssert( glAttachShader(g_program, fragmentShaderId) );
    glAssert(glBindAttribLocation(g_program, 0, "inPosition"));
    glAssert(glBindAttribLocation(g_program, 1, "inNormal"));
    glAssert(glBindAttribLocation(g_program, 2, "inTexCoord"));
    glAssert( glLinkProgram(g_program) );
    GLint linked;
    glAssert(glGetProgramiv(g_program, GL_LINK_STATUS, &linked));
    if (!linked) {
        std::cerr << "Program not linked" << std::endl;
        printProgramInfoLog(g_program);
    }
}

// -----------------------------------------------------------------------------

void Renderer::render(const glm::mat4x4 &modelViewMatrix,
                      const glm::mat4x4 &projectionMatrix)
{
    glAssert ( glClearColor ( 0.f, 0.3f, 0.3f, 0.f ) );
    glAssert ( glClear ( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT ) );

    glm::mat4x4 localprojectionMatrix = glm::perspective( 90.f, (float)mWidth/(float)mHeight, 0.1f, 200.0f );
    glm::mat4x4 localmodelViewMatrix = g_viewMatrix;

    glAssert( glUseProgram(g_program) );

    GLint transformationLoc[4];
    glAssert( transformationLoc[0] = glGetUniformLocation(g_program, "modelViewMatrix") );
    glAssert( transformationLoc[1] = glGetUniformLocation(g_program, "projectionMatrix") );
    glAssert( transformationLoc[2] = glGetUniformLocation(g_program, "normalMatrix") );
    glAssert( transformationLoc[3] = glGetUniformLocation(g_program, "MVP") );

    GLint materialLoc[4];
    glAssert( materialLoc[0] = glGetUniformLocation(g_program, "materialKd") );
    glAssert( materialLoc[1] = glGetUniformLocation(g_program, "materialKs") );
    glAssert( materialLoc[2] = glGetUniformLocation(g_program, "materialNs") );

    // Key light
    glm::vec3 keyLightPosition =  glm::vec3(localmodelViewMatrix*glm::vec4(-2.f, 2.f, -2.f, 1.f));
    glm::vec3 keyLightColor =  glm::vec3(0.8f, 0.2f, 0.2f); // Key light
    glAssert( glUniform3fv( glGetUniformLocation(g_program, "keyLightPosition"), 1, glm::value_ptr(keyLightPosition) ) );
    glAssert( glUniform3fv( glGetUniformLocation(g_program, "keyLightColor"), 1, glm::value_ptr(keyLightColor) ) );

    // Fill light
    glm::vec3 fillLightPosition =  glm::vec3(localmodelViewMatrix*glm::vec4(2.f, -2.f, 2.f, 1.f));
    glm::vec3 fillLightColor =  glm::vec3(0.2f, 0.8f, 0.2f);
    glAssert( glUniform3fv( glGetUniformLocation(g_program, "fillLightPosition"), 1, glm::value_ptr(fillLightPosition) ) );
    glAssert( glUniform3fv( glGetUniformLocation(g_program, "fillLightColor"), 1, glm::value_ptr(fillLightColor) ) );

    // Back light
    glm::vec3 backLightPosition =  glm::vec3(localmodelViewMatrix*glm::vec4(2.f, 2.f, -2.f, 1.f));
    glm::vec3 backLightColor =  glm::vec3(0.2f, 0.2f, 0.9f); // Back light
    glAssert( glUniform3fv( glGetUniformLocation(g_program, "backLightPosition"), 1, glm::value_ptr(backLightPosition) ) );
    glAssert( glUniform3fv( glGetUniformLocation(g_program, "backLightColor"), 1, glm::value_ptr(backLightColor) ) );

    // 4 - Dessiner les objets de la scène :
    for (std::vector<Entity*>::iterator it = g_entities.begin(); it != g_entities.end(); ++it ) {
        (*it)->drawGL(localmodelViewMatrix, localprojectionMatrix, transformationLoc, materialLoc);
    }
}

// -----------------------------------------------------------------------------

int Renderer::handleKeyEvent(char key) {

    switch (key) {
    case 'w':
        glAssert( glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) );
        break;
    case 'f':
        glAssert( glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) );
        break;
    }
    return 1;
}

// -----------------------------------------------------------------------------

int Renderer::handleMouseEvent(const MouseEvent & event){
    static int modifiers = 0;
    static int x = 0;
    static int y = 0;
    static int button = 0;
    static bool moved = false;

    if (event.click) {
        x = event.x;
        y = event.y;
        button = event.button;
        modifiers = event.modifiers;
    }

    if (event.button == MouseEvent::MOVE) {
        float dx = (float)(event.x - x) / (float) mWidth;
        float dy = (float)(event.y - y) / (float) mHeight;
        switch (button) {
        case MouseEvent::LEFT : {
            glm::vec3 xvec = glm::vec3(glm::row(g_viewMatrix, 0));
            glm::vec3 yvec = glm::vec3(glm::row(g_viewMatrix, 1));
            g_viewMatrix = glm::rotate(g_viewMatrix, dy*360.f, xvec);
            g_viewMatrix = glm::rotate(g_viewMatrix, dx*360.f, yvec);
        }
            break;
        case MouseEvent::RIGHT : {
            glm::vec3 xvec = glm::vec3(glm::row(g_viewMatrix, 0)) * dx * 10.f;
            glm::vec3 yvec = glm::vec3(glm::row(g_viewMatrix, 1)) * dy * 10.f;
            glm::vec3 tvec = xvec+yvec;
            g_viewMatrix = glm::translate(g_viewMatrix, tvec);
        }
            break;
        case MouseEvent::MIDDLE : {
            glm::vec3 zvec = glm::vec3(glm::row(g_viewMatrix, 2)) * dy * 10.f;
            g_viewMatrix = glm::translate(g_viewMatrix, zvec);
        }
            break;
        }
        x = event.x;
        y = event.y;
        moved = true;
        return 1;
    } else {
        moved = false;
    }

    if (event.button == MouseEvent::WHEEL) {
        return 1;
    }

    if (!event.click) {
        button = 0;
        modifiers = 0;
    }

    return 1;
}

// -----------------------------------------------------------------------------

void Renderer::setViewport(int width, int height) {
    mWidth = width;
    mHeight = height;
    glAssert( glViewport(0, 0, mWidth, mHeight) );
}

} // END NAMESPACE rendersystem ------------------------------------------------

// -----------------------------------------------------------------------------

typedef HRBF_fit<float, 3, Rbf_pow3<float> > HRBF;

// -----------------------------------------------------------------------------

void draw_cell(const MCube::GridCell& cell)
{
    int pairs[][2] = {{0,1}, {1,2}, {2,3}, {3,0}, // bottom square
                      {4,5}, {5,6}, {6,7}, {7,4}, // top square
                      {0,4}, {1,5}, {2,6}, {3,7}  // side edges
                     };
    const int nb_pairs = sizeof(pairs) / (sizeof(int)*2);
    for (int i = 0; i < nb_pairs; ++i) {
        int n = pairs[i][0];
        int m = pairs[i][1];
        display_list->vertex3f(cell.p[n].x, cell.p[n].y, cell.p[n].z);
        display_list->vertex3f(cell.p[m].x, cell.p[m].y, cell.p[m].z);
    }
}

// -----------------------------------------------------------------------------

void draw_samples(const std::vector<HRBF::Vector>& p,
                  const std::vector<HRBF::Vector>& n)
{
    assert( p.size() == n.size());
    display_list->begin(GL_POINTS);
    display_list->color3f(1.f, 0.f, 0.f);
    for(unsigned i = 0; i < p.size(); ++i)
        display_list->vertex3f(p[i](0), p[i](1), p[i](2));
    display_list->end();

    display_list->begin(GL_LINES);
    display_list->color3f(1.f, 0.f, 0.f);
    for(unsigned i = 0; i < n.size(); ++i){
        const HRBF::Vector p0 = p[i];
        const HRBF::Vector p1 = p0 + n[i];
        display_list->vertex3f(p0(0), p0(1), p0(2));
        display_list->vertex3f(p1(0), p1(1), p1(2));
    }
    display_list->end();
}

// -----------------------------------------------------------------------------

void draw_mesh_points(const rendersystem::MyGLMesh* mesh)
{
    display_list->begin(GL_POINTS);
    display_list->color3f(0.f, 0.f, 1.f);
    for(int i = 0; i < mesh->nbVertices(); ++i){
        glm::vec3 p = mesh->get_vertex(i).position;
        display_list->vertex3f(p.x, p.y, p.z);
    }
    display_list->end();

    display_list->begin(GL_LINES);
    display_list->color3f(0.f, 1.f, 0.f);
    for(int i = 0; i < mesh->nbVertices(); ++i){
        const glm::vec3 p0 = mesh->get_vertex(i).position;
        const glm::vec3 p1 = p0 + mesh->get_vertex(i).normal;
        display_list->vertex3f(p0.x, p0.y, p0.z);
        display_list->vertex3f(p1.x, p1.y, p1.z);
    }
    display_list->end();
}

// -----------------------------------------------------------------------------

rendersystem::MyGLMesh* make_hrbf(int res)
{
    HRBF fit0, fit1;


    HRBF::Vector pts0[] = { HRBF::Vector( 0.f, 0.f, 0.f), HRBF::Vector( 1.f, 0.f, 0.f), HRBF::Vector( 0.f, 0.f, 2.f)  };
    HRBF::Vector nrs0[] = { HRBF::Vector(-1.f, 0.f, 0.f), HRBF::Vector( 1.f, 0.f, 0.f), HRBF::Vector( 0.f, 0.f, 1.f)  };

    HRBF::Vector pts1[] = { HRBF::Vector( 0.f, 0.f, 0.f), HRBF::Vector(0.5f, 1.f, 0.f), HRBF::Vector(1.5f, 0.f, 0.f), HRBF::Vector(0.8f, 0.f, 1.f), HRBF::Vector(0.8f, 0.f, -3.f) };
    HRBF::Vector nrs1[] = { HRBF::Vector(-1.f, 0.f, 0.f), HRBF::Vector(0.0f, 1.f, 0.f), HRBF::Vector(1.0f, 0.f, 0.f), HRBF::Vector(0.0f, 0.f, 1.f), HRBF::Vector(0.0f, 0.f, -1.f) };

    const int size0 = sizeof(pts0) / sizeof(HRBF::Vector);
    std::vector<HRBF::Vector> p0(pts0, pts0 + size0 );
    std::vector<HRBF::Vector> n0(nrs0, nrs0 + size0 );

    const int size1 = sizeof(pts1) / sizeof(HRBF::Vector);
    std::vector<HRBF::Vector> p1(pts1, pts1 + size1 );
    std::vector<HRBF::Vector> n1(nrs1, nrs1 + size1 );

    draw_samples(p1, n1);

    // Interpole les points
    fit0.hermite_fit(p0, n0);
    fit1.hermite_fit(p1, n1);

    std::vector<float> verts;
    std::vector<int>   tris;
    verts.reserve( 10000   );
    tris .reserve( 10000*3 );

    display_list->begin(GL_LINES);
    display_list->color3f(0.2f, 0.2f, 0.2f);
    // Polygonise HRBF
    const float glen  = 6.f; /// Grid length
    const float clen  = glen / res;
    BBox::Point center(0.f, 0.f, 0.f ); /// center Bbox
    int acc = 0;
    for (int x = 0; x < res; ++x) {
        for (int y = 0; y < res; ++y) {
            for (int z = 0; z < res; ++z) {
                BBox::Point org( center.x + x * clen - glen*0.5f,
                                 center.y + y * clen - glen*0.5f,
                                 center.z + z * clen - glen*0.5f );

                MCube::GridCell cell;
                cell.p[0] = BBox::Point(org.x       , org.y       , org.z       );
                cell.p[1] = BBox::Point(org.x + clen, org.y       , org.z       );
                cell.p[2] = BBox::Point(org.x + clen, org.y       , org.z + clen);
                cell.p[3] = BBox::Point(org.x       , org.y       , org.z + clen);

                cell.p[4] = BBox::Point(org.x       , org.y + clen, org.z       );
                cell.p[5] = BBox::Point(org.x + clen, org.y + clen, org.z       );
                cell.p[6] = BBox::Point(org.x + clen, org.y + clen, org.z + clen);
                cell.p[7] = BBox::Point(org.x       , org.y + clen, org.z + clen);

                draw_cell( cell );

                for (int i = 0; i < 8; ++i)
                {
                    BBox::Point v = cell.p[i];
                    HRBF::Vector pos(v.x, v.y, v.z);
                    cell.val[i] = /*fit0.eval( pos ) + */fit1.eval( pos );
                }

                MCube::Triangle tri[5];
                int nb_tri = MCube::polygonise(cell, 0.0f, tri);

                for(int i = 0; i < nb_tri; ++i){
                    MCube::Triangle tr = tri[i];
                    for(int j = 0; j < 3; ++j) {
                        verts.push_back( tr.p[j].x );
                        verts.push_back( tr.p[j].y );
                        verts.push_back( tr.p[j].z );
                        tris.push_back( acc );
                        acc++;
                    }
                }
            }
        }
    }
    display_list->end();

    rendersystem::MyGLMesh* mesh = new rendersystem::MyGLMesh(verts, tris, false, false);

    // Compute normals with gradient:

    for(unsigned i = 0; i < (verts.size()/3); ++i)
    {
        HRBF::Vector p(verts[i*3], verts[i*3 + 1], verts[i*3 + 2]);
        HRBF::Vector n = /*fit0.grad( p ) + */fit1.grad( p );
        glm::vec3 v  = glm::vec3(n(0), n(1), n(2));
        v =  glm::normalize( glm::vec3(n(0), n(1), n(2)) );
        mesh->set_normal(i, v );
    }
    mesh->enable_normals(true);
    draw_mesh_points( mesh );

    return mesh;
}
