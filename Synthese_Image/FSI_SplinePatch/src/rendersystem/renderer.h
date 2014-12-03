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
#ifndef RENDERER_H
#define RENDERER_H
#include "opengl.h"

#include <vector>

#include "glmesh.h"
#include "glentity.h"
#include "gllight.h"

/** @defgroup RenderSystem Simple OpenGL Rendering system
 *  Simple OpenGL 3.2 core renderer.
 * @author Mathias Paulin <Mathias.Paulin@irit.fr>
 *  @{
 */

/**
  * Render system classes and functions.
  */
namespace rendersystem {

/**
  * @ingroup RenderSystem
  * Mouse event structure.
  * Mouse interaction events as the renderer will understand them.
  * It is the role of the graphical user interface to translate native mouse
  * event (Qt, WIN32, X11, ...) to this representation.
  */
struct MouseEvent {
    enum Modifiers {NONE = 0x00, CONTROL = 0x01, SHIFT = 0x02, ALT = 0x04};
    enum Buttons {LEFT = 0x01, MIDDLE = 0x02, RIGHT = 0x04, WHEEL = 0x08, MOVE = 0x10};
    int button;
    bool click; // true -> click, false -> release
    int modifiers;
    int x;
    int y;
    int delta;
};


/**
  * @ingroup RenderSystem
  * OpenGL renderer.
  * Manages all of the OpenGL operation to render a scene.
  */
class Renderer {
public:
    /**
     * Default constructor
     */
    Renderer() : g_program(0), g_viewMatrix(1.0f){}
    /**
     * Destructor
     */
    virtual ~Renderer() {}

    /**
     * VieViewport management.
     */
    virtual void setViewport(int width, int height);

    /**
     * Render the scene.
     */
    virtual void render(const glm::mat4x4 &modelViewMatrix, const glm::mat4x4 &projectionMatrix);
    virtual void renderDebug(const glm::mat4x4 &modelViewMatrix, const glm::mat4x4 &projectionMatrix);
    /**
     * Handle mouse event given by the vortexEngine
     * @return 1 if event is understood and fully managed. 0 otherwise.
     */
    virtual int handleMouseEvent(const MouseEvent & event);

    /**
     * Handle key event given by the vortexEngine
     * @return 1 if event is understood and fully managed. 0 otherwise.
     */
    virtual int handleKeyEvent(char key);

    /**
     * Initialise all the resources needed for rendering : shaders, geometries, teture, ...
     */
    virtual void initRessources();

    /**
     * Initialise the viewing configuration
     */
    virtual void initView();

    /**
     * Initialise the geometric content
     */
    virtual void initGeometry();

    /**
     * Initialise the lighting configuration
     */
    virtual void initLighting();

    /**
     * Initialise the shader configuration
     */
    virtual void initShaders();


    int width(){ return mWidth; }
    int height(){ return mHeight; }

    const std::vector<MyGlEntity*> &getEntities() {return g_entities;}

    const std::vector<MyGlLight*> &getLights() {return g_lights;}

    glm::mat4 getViewMatrix(){return g_viewMatrix;}
protected:
    int mWidth;
    int mHeight;

private:
    int initShader(std::string vsFileName, std::string fsFileName);
    /**
      * Vector of meshes of the scene.
      */
    std::vector<MyGLMesh*> g_meshes;

    /**
      * Vector of entities to be drawn.
      */
    std::vector<MyGlEntity*> g_entities;

    /**
      * Vector of lights.
      */
    std::vector<MyGlLight*> g_lights;

    /**
      * OpenGl Shader Program to be used when drawing.
      */
    int g_program;
    int g_programLine;

private:
    /**
      * Viewing matrix for the rendering.
      */
    glm::mat4 g_viewMatrix;

};

} // namespace rendersystem

/** @} */ // end of RenderSystem group

#endif

