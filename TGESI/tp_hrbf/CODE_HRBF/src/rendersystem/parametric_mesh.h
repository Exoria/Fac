#ifndef PARAMETRIC_MESH_HPP__
#define PARAMETRIC_MESH_HPP__

#include "fileloaders/mesh.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>

// =============================================================================
namespace rendersystem {
// =============================================================================

//-------------------------------------------
// TP3
//-------------------------------------------

//-------------------------------------------
// Classe ParametricMesh
//-------------------------------------------

class ParametricMesh : public loaders::Mesh {
public:
    ParametricMesh() : Mesh() { }

    void generateMesh(int res_u, int res_v)
    {
        assert(res_u > 0); assert(res_v > 0);
        mTriangles.clear();
        mVertices.clear();

        // Sample surface
        double step_u = 1. / (double)res_u;
        double step_v = 1. / (double)res_v;
        for(int j = 0; j < (res_v+1); j++)
            for(int i = 0; i < (res_u+1); i++)
                mVertices.push_back( evalSurface( ((double)i)*step_u, ((double)j)*step_v) );

        // Compute faces
        for(int i = 0; i < res_u; i++)
        {
            for(int j = 0; j < res_v; j++)
            {
                int a = toLinear(i  , j  , res_u+1);
                int b = toLinear(i+1, j  , res_u+1);
                int c = toLinear(i+1, j+1, res_u+1);
                int d = toLinear(i  , j+1, res_u+1);

                mTriangles.push_back( TriangleIndex(a, b, c) );
                mTriangles.push_back( TriangleIndex(a, c, d) );
            }
        }

        mNbVertices  = mVertices. size();
        mNbTriangles = mTriangles.size();

        mHasNormal        = true;
        mHasTextureCoords = true;
    }

protected:
    virtual Vertex evalSurface(float u, float v) const
    {
        assert(u >= 0.f); assert(u <= 1.f);
        assert(v >= 0.f); assert(v <= 1.f);

        Vertex vert;
        vert.position = glm::vec3(u-0.5f, v-0.5f, 0.f);
        vert.normal   = glm::vec3(0.f   , 0.f   , 1.f);
        vert.texcoord = glm::vec2(u, v);
        return vert;
    }

    int toLinear(int x, int y, int width) const{
        return y*width + x;
    }
};

//-------------------------------------------
// Classe ParametricSphere
//-------------------------------------------

class ParametricSphere : public ParametricMesh {
public:
    ParametricSphere() : ParametricMesh() { }

protected:
    Vertex evalSurface(float u, float v) const
    {
        assert(u >= 0.f); assert(u <= 1.f);
        assert(v >= 0.f); assert(v <= 1.f);

        Vertex vert;
        vert.texcoord = glm::vec2(u, v);

        u  = u * (2. * M_PI);
        v = v * M_PI;
        float x = cos(u)*sin(v);
        float y = sin(u)*sin(v);
        float z = cos(v);

        vert.position = glm::vec3(x, y, z);
        vert.normal   = glm::vec3(x, y, z);
        return vert;
    }
};

//-------------------------------------------
// Classe ParametricCylindre
//-------------------------------------------

class ParametricCylindre : public ParametricMesh {
public:
    ParametricCylindre() : ParametricMesh() { }

protected:
    Vertex evalSurface(float u, float v) const
    {
        assert(u >= 0.f); assert(u <= 1.f);
        assert(v >= 0.f); assert(v <= 1.f);

        Vertex vert;
        vert.texcoord = glm::vec2(u, v);

        u  = u * (2. * M_PI);

        float x = cos(u);
        float y = sin(u);
        float z = v;

        vert.position = glm::vec3(x, y, z  );
        vert.normal   = glm::vec3(x, y, 0.f);
        return vert;
    }
};

//-------------------------------------------
// Classe ParametricCone
//-------------------------------------------

class ParametricCone : public ParametricMesh {
public:
    ParametricCone() : ParametricMesh() { }

protected:
    Vertex evalSurface(float u, float v) const
    {
        assert(u >= 0.f); assert(u <= 1.f);
        assert(v >= 0.f); assert(v <= 1.f);

        Vertex vert;
        vert.texcoord = glm::vec2(u, v);

        u  = u * (2. * M_PI);

        float x = v*cos(u);
        float y = v*sin(u);
        float z = v;

        vert.position = glm::vec3(x, y, z  );
        vert.normal   = -glm::normalize( glm::cross(glm::vec3(cos(u), sin(u), 1.f), glm::vec3(-sin(u)*v, cos(u)*v, 0.f)));
        glm::normalize(vert.normal);
        return vert;
    }
};

}// END RENDERSYSTEME ==========================================================

#endif // PARAMETRIC_MESH_HPP__
