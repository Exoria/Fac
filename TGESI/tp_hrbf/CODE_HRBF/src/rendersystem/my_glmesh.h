#ifndef MY_GLMESH_HPP__
#define MY_GLMESH_HPP__

namespace rendersystem {

//-------------------------------------------
// TP1
//-------------------------------------------

//-------------------------------------------
// Classe MyGLMesh
//-------------------------------------------

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

/**
  * @ingroup RenderSystem
  * A mesh with OpenGL rendering capabilities.
  */
class MyGLMesh : public loaders::Mesh {
private :
    // OpenGL stuffs
    GLuint mVertexArrayObject;
    enum {VBO_VERTICES, VBO_INDICES};
    GLuint mVertexBufferObjects[2];

public:
    MyGLMesh (const loaders::Mesh &mesh) : loaders::Mesh(mesh){
    }

    MyGLMesh (const std::vector<float> &vertexBuffer,
              const std::vector<int> &triangleBuffer,
              bool hasNormals=true,
              bool hasTextureCoords=true )
        : loaders::Mesh(vertexBuffer,
                        triangleBuffer,
                        std::vector<int>() ,
                        hasNormals,
                        hasTextureCoords)
    {
    }

    /**
      * Build VartexArrayObjects for the mesh.
      */
    void compileGL() {


        glAssert( glGenVertexArrays(1, &mVertexArrayObject) );
        glAssert( glGenBuffers(2, mVertexBufferObjects) );

        glAssert( glBindVertexArray(mVertexArrayObject) );
        glAssert( glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObjects[VBO_VERTICES]) );
        glAssert( glBufferData(GL_ARRAY_BUFFER, mNbVertices * sizeof(Vertex),  &(mVertices[0]), GL_STATIC_DRAW) );

        GLuint stride = sizeof(Vertex);
        GLboolean normalized = GL_FALSE;
        GLenum type = GL_FLOAT;
        // position
        GLuint index = 0;
        GLint size = 3;
        GLvoid *pointer = BUFFER_OFFSET(0);
        glAssert( glVertexAttribPointer(index, size, type, normalized, stride, pointer) );
        glAssert( glEnableVertexAttribArray(index) );

        // normal
        index = 1;
        size = 3;
        pointer = BUFFER_OFFSET(sizeof(glm::vec3));
        glAssert( glVertexAttribPointer(index, size, type, normalized, stride, pointer) );
        glAssert( glEnableVertexAttribArray(index) );

        // texcoord
        index = 2;
        size = 2;
        pointer = BUFFER_OFFSET(2*sizeof(glm::vec3));
        glAssert( glVertexAttribPointer(index, size, type, normalized, stride, pointer) );
        glAssert( glEnableVertexAttribArray(index) );

        glAssert( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVertexBufferObjects[VBO_INDICES]) );
        glAssert( glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*mNbTriangles * sizeof(int),  &(mTriangles[0]), GL_STATIC_DRAW) );

        glAssert( glBindVertexArray(0) );
    }

    /// Draws the VartexArrayObjects of the mesh.
    void drawGL()
    {
        glAssert( glBindVertexArray(mVertexArrayObject) );
        glAssert( glDrawElements(GL_TRIANGLES, 3*mNbTriangles, GL_UNSIGNED_INT, NULL) );

    }

};

}// END RENDERSYSTEM ===========================================================

#endif // MY_GLMESH_HPP__
