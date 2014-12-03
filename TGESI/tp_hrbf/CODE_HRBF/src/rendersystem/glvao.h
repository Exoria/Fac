#ifndef GLVAO_HPP__
#define GLVAO_HPP__

//#include "port_glew.h"
#include "opengl.h"
#include "glassert.h"

#include "glbuffer_object.h"

class GlVao {
public:

    GlVao(){ glAssert( glGenVertexArrays(1, &_id) ); }

    ~GlVao(){ glDeleteVertexArrays(1, &_id); }

    void bind(){ glAssert( glBindVertexArray(_id) ); }

    static void unbind(){ glBindVertexArray(0); }

    /// @param vbo_id : opengl identifier of the vertex object buffer to
    /// be recorded by the VAO
    /// @param attr_idx : index of the attribute
    /// @param nb_components : number of components (x, y, z ...) of the attribute
    /// @warning you must bind the vao buffer using this method
    void record_attr(GLuint vbo_id, int attr_idx, int nb_components){
        glAssert( glBindBuffer(GL_ARRAY_BUFFER, vbo_id) );
        glAssert( glVertexAttribPointer(attr_idx, nb_components, GL_FLOAT, GL_FALSE, 0, 0) );
        glAssert( glEnableVertexAttribArray(attr_idx) );
    }

    /// @warning you must bind the vao buffer using this method
    void record_elt(GLuint vbo_elts_id){
        glAssert( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elts_id) );
    }

private:
    GLuint _id;
};

#endif // GLVAO_HPP__
