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
#ifndef MESH_H
#define MESH_H


#include <vector>
#include "glm/glm.hpp"

namespace loaders {
/**
  *  @ingroup Loaders
  *
  * Represents a triangular mesh as an array of vertex and an array of triangluar faces.
  * In the vertex buffer, datas are interleaved (x,y,z,nx,ny,nz,u,v) when auxiliary data are present ((nx,ny,nz) or (u,v))
  */
class Mesh {
public:
    /**
      * Internal vertex representation : position (vec3), normal (vec3) and texcoord (vec2)
      */
    class Vertex {
    public:
        Vertex (void) : position (glm::vec3(0.,0.,0.)), normal (glm::vec3(0.,0.,0.)), texcoord(glm::vec2(0.,0.)) {}
        Vertex (const glm::vec3& pos) : position (pos), normal (glm::vec3(0.,0.,0.)), texcoord(glm::vec2(0.,0.)) {}

        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texcoord;
    };

    /**
      * Default constructor.
      * Creates an empty mesh.
      */
    Mesh ();

    /**
      * Constructor from basic geometric data.
      * Creates the triangluar mesh from a vertex array and triangle or quad array.
      * Vertices are described by (x,y,z,nx,ny,nz,u,v), (x,y,z,nx,ny,nz) or (x,y,z)
      * depending of the values of hasNormals and hasTextureCoords.
      */
    Mesh (const std::vector<float> &vertexBuffer,
          const std::vector<int> &triangleBuffer,
          const std::vector<int> &quadBuffer,
          bool hasNormals, bool hasTextureCoords
         );

    /**
      * Copy contructor.
      */
    Mesh(const Mesh &mesh) {
        mVertices = mesh.mVertices;
        mTriangles = mesh.mTriangles;
        mNbVertices = mesh.mNbVertices;
        mNbTriangles = mesh.mNbTriangles;
        mHasTextureCoords = mesh.mHasTextureCoords;
        mHasNormal = mesh.mHasNormal;
    }

    /**
      * Destructor.
      */
    virtual ~Mesh();

    /**
      * Gets the mesh data in raw format.
      */
    void getData ( std::vector<float> &vertexBuffer,
           std::vector<int> &triangleBuffer,
           bool &parametrized
         );

    Vertex get_vertex(int i) const { return mVertices[i]; }

    /**
      * Concatenates 2 meshes.
      */
    Mesh & operator+=(const Mesh &m);

    /**
      * Returns the number of vertices in the mesh
      */
    int nbVertices () const {
            return mNbVertices;
    }

    /**
      * Returns the number of vertices in the mesh
      */
    int nbTriangles () const {
            return mNbTriangles;
    }

    /**
      * Prints basic information about the mesh on stderr.
      */
    void printfInfo() const;

    void set_normal(int i, const glm::vec3& n) {
        mVertices[i].normal = n;
    }

    void enable_normals(bool s){
        mHasNormal = s;
    }



protected:
    /**
      * Internal triangle representation : three integer indices
      */
    class TriangleIndex {
    public:
        TriangleIndex ( ){}
        TriangleIndex (int i0, int i1, int i2) {
                indexes[0] = i0;
                indexes[1] = i1;
                indexes[2] = i2;
        };
        TriangleIndex (const int* pIds) {
                indexes[0] = pIds[0];
                indexes[1] = pIds[1];
                indexes[2] = pIds[2];
        };
        unsigned int indexes[3];
        inline unsigned int& operator[] (int k) {
                assert (k<3);
                return indexes[k];
        }
        inline unsigned int operator[] (int k) const {
                assert (k<3);
                return indexes[k];
        }
    };

    typedef std::vector<Vertex> VertexArray;
    typedef std::vector<TriangleIndex> TriangleIndexArray;

    VertexArray mVertices;
    int mNbVertices;

    TriangleIndexArray mTriangles;
    int mNbTriangles;

    bool mHasTextureCoords;
    bool mHasNormal;

    /**
      * Compute smothed normals at each vertex.
      */
    void computeNormals (void);

};

} // namespace loaders
#endif
