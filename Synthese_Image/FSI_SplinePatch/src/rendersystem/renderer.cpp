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

#include "glmaterial.h"
#include "parametricmeshes.h"

#include "spline_stuff.h"

#include <glm/gtc/matrix_access.hpp>

namespace rendersystem {
BSpline3D *bspline;
BSplinePatch *patch;

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



//-------------------------------------------
// Méthodes de Renderer
//-------------------------------------------

//-------------------------------------------
// Initialisation des resources globales
//-------------------------------------------
void Renderer::initRessources(){
    {
        glAssert( glEnable(GL_DEPTH_TEST) );
        glAssert( glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) );
    }
    initView();
    initGeometry();
    initLighting();
    initShaders();
}

//-------------------------------------------
// Initialisation des paramètres de vue.
//-------------------------------------------
void Renderer::initView(){
    glm::vec3 eye(1.f, 0.25f, 1.f);
    glm::vec3 center(0.0f, 0.0f, 0.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    g_viewMatrix = glm::lookAt(eye,center,up);
}

//-------------------------------------------
// Chargement et compilation des données géométriques de l'application.
//-------------------------------------------
#define SHOW_FRAME
void Renderer::initGeometry(){

    MyGLMaterial *defaultMaterial = new MyGLMaterial(glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), 25.f);
    Texture *defaultTex = new Texture("../data/rustbin.png");
    defaultTex->useMipMap(GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR);
    defaultMaterial->setTexture(defaultTex);

    ParametricMesh* sphere = new ParametricSphere();
    ParametricMesh* cylinder = new ParametricCylindre();
    ParametricMesh* cone   = new ParametricCone();

    sphere->generateMesh(50,  50 );
    cylinder->generateMesh(20, 10);
    cone->generateMesh(20, 10);


    MyGLMesh *glSphere = new MyGLMesh(*sphere);
    MyGLMesh *glCylinder = new MyGLMesh(*cylinder);
    MyGLMesh *glCone = new MyGLMesh(*cone);

    g_meshes.push_back( glSphere );
    g_meshes.push_back( glCylinder );
    g_meshes.push_back( glCone );

    delete sphere;
    delete cylinder;
    delete cone;



#define SHOWCURVE
#ifdef SHOWCURVE
    {
        std::vector< glm::vec3 > b_pts;
        b_pts.push_back(glm::vec3(0, 0, 0));
        b_pts.push_back(glm::vec3(1, 0, 0));
        b_pts.push_back(glm::vec3(2, 0, 1));
        b_pts.push_back(glm::vec3(2, 0, 2));
        b_pts.push_back(glm::vec3(3, 0, 3));
        b_pts.push_back(glm::vec3(4, 0, 3));
        int curve_substep = 32;
        glm::mat4 theTransformation(1.0f);
        theTransformation = glm::translate(theTransformation, glm::vec3(-2, 2, -2));
        bspline = new BSpline3D(b_pts, 3, Uniform, curve_substep, theTransformation);
        bspline->compileGL();
    }
#endif



#define SHOWPATCH
#ifdef SHOWPATCH
    {
        std::vector<glm::vec3> controlMesh;
        controlMesh.push_back(glm::vec3(0.f, 0.f, 0.f));
        controlMesh.push_back(glm::vec3(0.f, 1.f, 0.f));
        controlMesh.push_back(glm::vec3(0.f, 2.f, 0.f));
        controlMesh.push_back(glm::vec3(0.f, 3.f, 0.f));
        controlMesh.push_back(glm::vec3(0.f, 3.5f, 0.f));

        controlMesh.push_back(glm::vec3(1.f, 0.f, 0.f));
        controlMesh.push_back(glm::vec3(1.f, 1.f, 1.7f));
        controlMesh.push_back(glm::vec3(1.f, 2.f, 1.f));
        controlMesh.push_back(glm::vec3(1.f, 3.f, 0.f));
        controlMesh.push_back(glm::vec3(1.f, 4.f, 0.f));

        controlMesh.push_back(glm::vec3(2.f, 0.f, 1.f));
        controlMesh.push_back(glm::vec3(2.f, 1.f, 1.6f));
        controlMesh.push_back(glm::vec3(2.f, 2.f, 1.f));
        controlMesh.push_back(glm::vec3(2.f, 3.f, 1.f));
        controlMesh.push_back(glm::vec3(2.f, 4.f, 0.f));

        controlMesh.push_back(glm::vec3(3.f, 0.f, 2.f));
        controlMesh.push_back(glm::vec3(3.f, 1.f, 1.4f));
        controlMesh.push_back(glm::vec3(3.f, 2.f, 2.6f));
        controlMesh.push_back(glm::vec3(3.f, 3.f, 1.f));
        controlMesh.push_back(glm::vec3(3.f, 3.5f, 0.f));

        controlMesh.push_back(glm::vec3(4.f, 0.f, 2.f));
        controlMesh.push_back(glm::vec3(4.f, 1.f, 1.4f));
        controlMesh.push_back(glm::vec3(4.f, 2.f, 1.6f));
        controlMesh.push_back(glm::vec3(4.f, 3.f, 1.f));
        controlMesh.push_back(glm::vec3(4.f, 2.f, 0.f));

        controlMesh.push_back(glm::vec3(5.f, 0.f, 2.f));
        controlMesh.push_back(glm::vec3(5.f, 1.f, 1.4f));
        controlMesh.push_back(glm::vec3(5.f, 2.f, 1.6f));
        controlMesh.push_back(glm::vec3(5.f, 3.f, 1.f));
        controlMesh.push_back(glm::vec3(5.f, 2.f, 0.f));

        controlMesh.push_back(glm::vec3(6.f, 0.f, 2.f));
        controlMesh.push_back(glm::vec3(6.f, 1.f, 1.4f));
        controlMesh.push_back(glm::vec3(6.f, 2.f, 1.6f));
        controlMesh.push_back(glm::vec3(6.f, 3.f, 1.f));
        controlMesh.push_back(glm::vec3(6.f, 2.f, 0.f));

        glm::mat4 theTransformation(1.0f);
        theTransformation = glm::mat4(1.0f);
        theTransformation = glm::rotate(theTransformation, -90.0f, glm::vec3(1.f,0.f,0.f));
        theTransformation = glm::scale(theTransformation, glm::vec3(1.f,1.f,.5f));
        theTransformation = glm::translate(theTransformation, glm::vec3(-2.f,-2.f, -1.5f));

        patch = new BSplinePatch(4, 4, OpenUniform, controlMesh, 5, 7, 32, 32, theTransformation);
        patch->compileGL();

        ParametricMesh* paramPatch   = new ParametricBSplinePatch(patch);
        paramPatch->generateMesh( 32, 32);
        MyGLMesh *glPatch = new MyGLMesh(*paramPatch);
        g_meshes.push_back( glPatch);
        delete paramPatch;

        MyGlEntity *newEntity = new MyGlEntity(glPatch, defaultMaterial, theTransformation);
        g_entities.push_back(newEntity);
    }
#endif

#ifdef SHOW_FRAME
    {
        // Construction du repère

        glm::mat4 theTransformation = glm::scale(glm::mat4(1.0f), glm::vec3(1.f,1.f,1.f));

        // La sphère pour noter l'Origine
        MyGLMaterial *centerMaterial = new MyGLMaterial(glm::vec3(1.f, 1.f, 0.01f), glm::vec3(1.0f, 1.0f, 1.0f), 25.f);
        unsigned char yellow[3];
        yellow[0]=yellow[1]=255;
        yellow[2]=2;
        Texture *centerTex = new Texture(yellow);
        centerMaterial->setTexture(centerTex);

        glm::mat4 sphereTransformation = glm::scale(theTransformation, glm::vec3(0.1f,0.1f,0.1f));
        MyGlEntity * newEntity = new MyGlEntity(glSphere, centerMaterial, sphereTransformation);
        g_entities.push_back(newEntity);

        // axe Z
        {
            MyGLMaterial *zAxisMaterial = new MyGLMaterial(glm::vec3(0.01f, 0.01f, 1.0f), glm::vec3(1.f, 1.f, 1.f), 50.f);

            unsigned char blue[3];
            blue[0]=blue[1]=2;
            blue[2]=255;
            Texture *theTex = new Texture(blue);
            zAxisMaterial->setTexture(theTex);

            glm::mat4 cylinderTransformation = glm::scale(theTransformation, glm::vec3(0.03f, 0.03f, 0.9f));
            newEntity = new MyGlEntity(glCylinder, zAxisMaterial, cylinderTransformation);
            g_entities.push_back(newEntity);

            glm::mat4 coneTransformation = glm::rotate(theTransformation, 180.0f, glm::vec3(0.f, 1.f, 0.f));
            coneTransformation = glm::scale(coneTransformation, glm::vec3(0.06f,0.06f,0.2f));
            coneTransformation = glm::translate(coneTransformation, glm::vec3(0.f,0.f,-5.f));

            newEntity = new MyGlEntity(glCone, zAxisMaterial, coneTransformation);
            g_entities.push_back(newEntity);
        }

        // axe X
        theTransformation = glm::rotate(theTransformation, 90.0f, glm::vec3(0.f, 1.f, 0.f));
        {
            MyGLMaterial *xAxisMaterial = new MyGLMaterial(glm::vec3(1.0f, 0.01f, 0.01f), glm::vec3(1.f, 1.f, 1.f), 50.f);

            unsigned char red[3];
            red[2]=red[1]=2;
            red[0]=255;
            Texture *theTex = new Texture(red);
            xAxisMaterial->setTexture(theTex);

            glm::mat4 cylinderTransformation = glm::scale(theTransformation, glm::vec3(0.03f, 0.03f, 0.9f));
            newEntity = new MyGlEntity(glCylinder, xAxisMaterial, cylinderTransformation);
            g_entities.push_back(newEntity);

            glm::mat4 coneTransformation = glm::rotate(theTransformation, 180.0f, glm::vec3(0.f, 1.f, 0.f));
            coneTransformation = glm::scale(coneTransformation, glm::vec3(0.06f,0.06f,0.2f));
            coneTransformation = glm::translate(coneTransformation, glm::vec3(0.f,0.f,-5.f));

            newEntity = new MyGlEntity(glCone, xAxisMaterial, coneTransformation);
            g_entities.push_back(newEntity);
        }

        // axe Y
        theTransformation = glm::rotate(theTransformation, -90.0f, glm::vec3(1.f, 0.f, 0.f));
        {
            MyGLMaterial *yAxisMaterial = new MyGLMaterial(glm::vec3(0.01f, 1.f, 0.01f), glm::vec3(1.f, 1.f, 1.f), 50.f);

            unsigned char green[3];
            green[0]=green[2]=2;
            green[1]=255;
            Texture *theTex = new Texture(green);
            yAxisMaterial->setTexture(theTex);

            glm::mat4 cylinderTransformation = glm::scale(theTransformation, glm::vec3(0.03f, 0.03f, 0.9f));
            newEntity = new MyGlEntity(glCylinder, yAxisMaterial, cylinderTransformation);
            g_entities.push_back(newEntity);

            glm::mat4 coneTransformation = glm::rotate(theTransformation, 180.0f, glm::vec3(0.f, 1.f, 0.f));
            coneTransformation = glm::scale(coneTransformation, glm::vec3(0.06f,0.06f,0.2f));
            coneTransformation = glm::translate(coneTransformation, glm::vec3(0.f,0.f,-5.f));

            newEntity = new MyGlEntity(glCone, yAxisMaterial, coneTransformation);
            g_entities.push_back(newEntity);
        }
    }
#endif

    for (std::vector<MyGLMesh*>::iterator it = g_meshes.begin(); it != g_meshes.end(); ++it ) {
        (*it)->compileGL();
    }
}

//-------------------------------------------
// Chargement et compilation des shaders
//-------------------------------------------

int Renderer::initShader(std::string vsFileName, std::string fsFileName){
    char* vertexShaderSource = loaders::text::loadFile(vsFileName.c_str());
    int vertexShaderId;
    glAssert(vertexShaderId = glCreateShader(GL_VERTEX_SHADER));
    glAssert( glShaderSource(vertexShaderId, 1, (const GLchar **)&vertexShaderSource, NULL) );
    glAssert(glCompileShader(vertexShaderId));
    GLint compiled;
    glAssert(glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &compiled));
    if (!compiled) {
        std::cerr << "[" << vsFileName << "] Vertex shader not compiled : " << std::endl;
        printShaderInfoLog(vertexShaderId);
        return 0;
    }
    char *fragmentShaderSource;
    fragmentShaderSource = loaders::text::loadFile(fsFileName.c_str());
    int fragmentShaderId;
    glAssert(fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER));
    glAssert( glShaderSource(fragmentShaderId, 1, (const GLchar **)&fragmentShaderSource, NULL) );
    glAssert(glCompileShader(fragmentShaderId));
    glAssert(glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &compiled));
    if (!compiled) {
        std::cerr << "[" << fsFileName << "] fragment shader not compiled : " << std::endl;
        printShaderInfoLog(fragmentShaderId);
        return 0;
    }
    int program;
    glAssert(program = glCreateProgram());
    glAssert( glAttachShader(program, vertexShaderId) );
    glAssert( glAttachShader(program, fragmentShaderId) );
    glAssert(glBindAttribLocation(program, 0, "inPosition"));
    glAssert(glBindAttribLocation(program, 1, "inNormal"));
    glAssert(glBindAttribLocation(program, 2, "inTexCoord"));
    glAssert( glLinkProgram(program) );
    GLint linked;
    glAssert(glGetProgramiv(program, GL_LINK_STATUS, &linked));
    if (!linked) {
        std::cerr << "Program not linked" << std::endl;
        printProgramInfoLog(program);
    }
    return program;
}

void Renderer::initShaders(){
    g_program = initShader("../shaders/vertexdefault.glsl", "../shaders/fragmentdefault.glsl");
    g_programLine = initShader("../shaders/vertexline.glsl", "../shaders/fragmentline.glsl");
}


//-------------------------------------------
// Fonction générale de rendu
//-------------------------------------------
void Renderer::render(const glm::mat4x4 &modelViewMatrix, const glm::mat4x4 &projectionMatrix){
    glAssert ( glClearColor ( 0.f, 0.3f, 0.3f, 0.f ) );
    glAssert ( glClear ( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT ) );
    glm::mat4x4 localprojectionMatrix = glm::perspective	( 90.f, (float)mWidth/(float)mHeight, 0.1f, 10.0f );
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
    glAssert( materialLoc[3] = glGetUniformLocation(g_program, "baseTexture") );

    glAssert( glUniform3fv( glGetUniformLocation(g_program, "keyLightPosition"), 1, glm::value_ptr(g_lights[0]->getPosition())) );
    glAssert( glUniform3fv( glGetUniformLocation(g_program, "keyLightColor"), 1, glm::value_ptr(g_lights[0]->getColor())) );
    glAssert( glUniform3fv( glGetUniformLocation(g_program, "fillLightPosition"), 1, glm::value_ptr(g_lights[1]->getPosition())) );
    glAssert( glUniform3fv( glGetUniformLocation(g_program, "fillLightColor"), 1, glm::value_ptr(g_lights[1]->getColor())) );
    glAssert( glUniform3fv( glGetUniformLocation(g_program, "backLightPosition"), 1, glm::value_ptr(g_lights[2]->getPosition())) );
    glAssert( glUniform3fv( glGetUniformLocation(g_program, "backLightColor"), 1, glm::value_ptr(g_lights[2]->getColor())) );

    for (std::vector<MyGlEntity*>::iterator it = g_entities.begin(); it != g_entities.end(); ++it ) {
        (*it)->drawGL(localmodelViewMatrix, localprojectionMatrix, transformationLoc, materialLoc);
    }
}

//-------------------------------------------
// Fonction de rendu annexe
//-------------------------------------------
void Renderer::renderDebug(const glm::mat4x4 &modelViewMatrix, const glm::mat4x4 &projectionMatrix){
    glm::mat4x4 localprojectionMatrix = glm::perspective( 90.f, (float)mWidth/(float)mHeight, 0.1f, 10.0f );
    glm::mat4x4 localmodelViewMatrix = g_viewMatrix;

    glAssert( glUseProgram(g_programLine) );

    GLint transformationLoc[4];
    glAssert( transformationLoc[0] = glGetUniformLocation(g_programLine, "modelViewMatrix") );
    glAssert( transformationLoc[1] = glGetUniformLocation(g_programLine, "projectionMatrix") );
    glAssert( transformationLoc[2] = glGetUniformLocation(g_programLine, "normalMatrix") );
    glAssert( transformationLoc[3] = glGetUniformLocation(g_programLine, "MVP") );
    GLint colorLocation;
    glAssert( colorLocation = glGetUniformLocation(g_programLine, "color") );

#ifdef SHOWCURVE
    bspline->drawGL(localmodelViewMatrix, localprojectionMatrix, transformationLoc, colorLocation);
#endif

#ifdef SHOWPATCH
    patch->drawGL(localmodelViewMatrix, localprojectionMatrix, transformationLoc, colorLocation);
#endif
}


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

void Renderer::setViewport(int width, int height) {
    mWidth = width;
    mHeight = height;
    glAssert( glViewport(0, 0, mWidth, mHeight) );
}

void Renderer::initLighting(){
    glm::vec3 keyLightPosition =  glm::vec3(0., 2.f, 0.f);
    glm::vec3 keyLightColor =  glm::vec3(0.6f, 0.7f, 0.8f);

    glm::vec3 fillLightPosition =  glm::vec3(1.f, 0.5f, 0.f);
    glm::vec3 fillLightColor = 0.75f * keyLightColor;

    glm::vec3 backLightPosition =  glm::vec3(-1.f, .5f, -2.f);
    glm::vec3 backLightColor =  0.75f * keyLightColor;

    MyGlLight *light;
    light = new MyGlLight(keyLightPosition, keyLightColor, true);
    g_lights.push_back(light);

    light = new MyGlLight(fillLightPosition, fillLightColor, true);
    g_lights.push_back(light);

    light = new MyGlLight(backLightPosition, backLightColor, false);
    g_lights.push_back(light);
}

} // namespace rendersystem
