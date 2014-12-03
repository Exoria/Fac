#ifndef ICOSAHEDRON_HPP__
#define ICOSAHEDRON_HPP__

#include "mesh.h"
#include <cmath>
#include <list>
#include <vector>
#include <cassert>

// =============================================================================
namespace rendersystem {
// =============================================================================

class Icosahedron : public loaders::Mesh {
public:
    Icosahedron();

protected:
    Vertex makeVertex(float theta, float phi) const;
};

// =============================================================================

Icosahedron::Icosahedron() : Mesh() {
    // Construct an icosaheedron using spherical coordinates
    // Hint : see topology in the picture  http://upload.wikimedia.org/wikipedia/commons/thumb/d/dd/Icosahedron_flat.svg/1000px-Icosahedron_flat.svg.png
    // Excerpt and adapted from http://en.wikipedia.org/wiki/Icosahedron#Spherical_Coordinates
    // Two vertices (top most and bottom most on the picture, numbered 10 and 11) are placed on the poles of a sphere,
    // the remaining vertices are located at latitude ±arctan(1/2). The longitudes can be found using the symmetry
    // of the remaining ten vertices by fixing a point then rotating 36° about the polar axis and reflecting
    // about the equator until we return to the original point. These points, numbered 0 to 9 are the vertices on
    // the middle triangle strip of the picture, numbered from left to right.
    // Triangles are made of the side strip plus by linking north pole (vertex 10) to even vertices of the strip and
    // south pole (vertex 11) to odd vertices of the strip.
    // Vertices
    float theta = atan(1.f/2.f);
    float phi = 0;
    float incPhi = M_PI/5.f;
    for (int i=0; i<5; i++){
        mVertices.push_back( makeVertex(theta, phi) );
        phi += incPhi;
        mVertices.push_back( makeVertex(-theta, phi) );
        phi += incPhi;
    }
    mVertices.push_back(makeVertex(M_PI_2, 0.f));
    mVertices.push_back(makeVertex(-M_PI_2, 0.f));
    // Faces
    for (int i=0; i<10; i+=2){
        mTriangles.push_back( TriangleIndex(i, (i+1)%10, (i+2)%10 ) );
        mTriangles.push_back( TriangleIndex((i+1)%10, (i+3)%10, (i+2)%10 ) );
    }
    for (int i=0; i<10; i+=2){
        mTriangles.push_back( TriangleIndex(10, i, (i+2)%10 ) );
        mTriangles.push_back( TriangleIndex(11, (i+3)%10, (i+1)%10) );
    }
    mNbVertices  = mVertices. size();
    mNbTriangles = mTriangles.size();
    mHasNormal        = true;
    mHasTextureCoords = true;

}

// -----------------------------------------------------------------------------

loaders::Mesh::Vertex Icosahedron::makeVertex(float theta, float phi) const {
    Vertex vert;
    // changement de référentiel en theta
    theta = M_PI_2 - theta;
    vert.texcoord = glm::vec2(theta/M_PI, phi/(2*M_PI) );
    float x = cos(phi)*sin(theta);
    float y = sin(phi)*sin(theta);
    float z = cos(theta);
    vert.position = glm::vec3(x, y, z);
    vert.normal   = glm::vec3(x, y, z);
    return vert;
}

// =============================================================================

class IcoSphere : public loaders::Mesh {
public:
    /// Build an Icosphere from a icosahedron subdivided n times
    IcoSphere(const Icosahedron& ico, int n);

private:
    /// @brief Structure of an edge shared between two triangles
    struct Edge {

        Edge(){}
        Edge(unsigned v0, unsigned  v1){
            v[0] = v0;
            v[1] = v1;
            mid  = 0; sub_e[0] = 0; sub_e[1] = 0;
        }

        unsigned v[2];  ///< vertices idx
        unsigned mid;   ///< midlle vertex once the edge is split
        Edge* sub_e[2]; ///< Pointer to the two sub edges (or zero if not created)

        bool operator ==(const Edge& e) const {
            return (v[0] == e.v[0] && v[1] == e.v[1]) ||
                   (v[0] == e.v[1] && v[1] == e.v[0]);
        }
    };


    struct Tri {
        Tri(unsigned a_, unsigned b_, unsigned c_,
            Edge*   e0_, Edge*    e1_, Edge* e2_ )
        {
            v[0] = a_;  v[1] = b_;  v[2] = c_;
            e[0] = e0_; e[1] = e1_; e[2] = e2_;
        }

        unsigned operator[] (int k) const {
                assert( k < 3);
                return v[k];
        }

        unsigned v[3]; ///< vertices idx
        Edge*    e[3]; ///< three adjacent edges
    };

    /// std::list are very efficient to insert/erase elements.
    /// However direct access of the ith element is is not available
    /// but we only need to look up linearly the array so that's not
    /// a problem in our case
    std::list<Tri> mTri;

    std::vector<Edge> mEdges;
    int nbEdges;

    /// Find an edge from the list of edges. If not available
    /// create it and add it to the list.
    /// @warning highly inefficient this method is intend to be used
    /// for initializion when the number of edges is low
    Edge* find_edge(unsigned v0, unsigned v1);

    /// We build the topology defined by our inner structures 'Tri' and
    /// 'Edges'. This basically means filling out the 'mTri' and 'mEdges'
    /// attributes. This algorithm does not intend to be efficient since the
    /// icosahedron is a very small primitive.
    void build_topology(const Icosahedron& ico);

    /// Result of the subdivision is duplicated into the compact
    /// array 'mTriangles'.
    /// intermediate results are dropped ('mTri' and 'mEdges' gets erased)
    void copy_result();

    /// @return uv coordinates of the sphere considering it is
    /// at the origin
    glm::vec2 get_uv(const glm::vec3& pos) const;

    /// Add a mid point two the edge 'e' of triangle tr.
    /// If the mid point doesn't exists we create it
    /// and add the two coresponding sub edges
    /// @return index of the mid point
    unsigned add_mid(Tri& tr, int edgeIdx, std::vector<Edge>& newEdges, int& nbNewEdges);

    /// @return the sub edge connected to the vertex vidx
    /// @warning vidx must not be the mid point
    Edge* find_sub_edge(const Edge* e, unsigned vidx) const;

};

// =============================================================================

// -----------------------------------------------------------------------------

// this is the only way to force an std container to effectively drop
// its elements. A vec.clear() does not guarantee that memory will be
// erased right after the call.
template < class T >
void erase_list(std::list<T>& list){
    std::list<T> empty;
    empty.swap( list );
}

// -----------------------------------------------------------------------------

template < class T >
void erase_vec(std::vector<T>& vec){
    std::vector<T> empty;
    empty.swap( vec );
}

// -----------------------------------------------------------------------------

static int ipow(int x, int p)
{
    assert(p >= 0);
    if (p == 0) return 1;
    if (p == 1) return x;
    return x * ipow(x, p-1);
}

// -----------------------------------------------------------------------------

IcoSphere::IcoSphere(const Icosahedron& ico, int n)
{
    build_topology(ico);

    for(int depth = 0; depth < n; depth++)
    {
        // from euler formula Vert-Edges+Faces = (1-(genus=0))*2
        mNbVertices = 2 + (20 * ipow(4, depth+1)) / 2;
        mVertices.reserve( mNbVertices );

        std::vector<Edge> newEdges;
        newEdges.resize( nbEdges * 2 + mTri.size() * 3);
        int nbNewEdges = 0;

        std::list<Tri>::iterator it = mTri.begin();
        while( it != mTri.end() )
        {
            Tri& tr = *it;
            // Compute mid vertices
            unsigned mid[3];
            for(int i = 0; i < 3; i++)
                mid[i] = add_mid(tr, i, newEdges, nbNewEdges);

            // Create the edges at the interior of the subdivided triangle
            Edge* innerEdges[3];
            for(int i = 0; i < 3; i++)
            {
                newEdges[nbNewEdges++] = Edge(mid[i], mid[(i+1)%3]);
                innerEdges[i] = &(newEdges[ nbNewEdges - 1]);
            }

            // Add four new triangles
            Tri t(mid[0], tr[1], mid[1], find_sub_edge(tr.e[0], tr[1]), find_sub_edge(tr.e[1], tr[1]), innerEdges[0]);
            mTri.push_front( t );
            t = Tri(mid[2], mid[1], tr[2], innerEdges[1], find_sub_edge(tr.e[1], tr[2]), find_sub_edge(tr.e[2], tr[2]));
            mTri.push_front( t );
            t = Tri(tr[0], mid[0], mid[2], find_sub_edge(tr.e[0], tr[0]), innerEdges[2], find_sub_edge(tr.e[2], tr[0]));
            mTri.push_front( t );
            t = Tri(mid[0], mid[1], mid[2], innerEdges[0], innerEdges[1], innerEdges[2]);
            mTri.push_front( t );

            // Erase the triangle which is subdivided
            it = mTri.erase( it );
        }

        assert(nbNewEdges == (int)newEdges.size());
        // Swap with new edges and erase old edges
        nbEdges = nbNewEdges;
        mEdges.swap( newEdges );
        erase_vec( newEdges );
    }

    copy_result();
}

// -----------------------------------------------------------------------------

IcoSphere::Edge* IcoSphere::find_edge(unsigned v0, unsigned v1)
{
    Edge e(v0, v1);
    for(int i = 0; i < nbEdges; i++)
        if( mEdges[i] == e )
            return &(mEdges[i]);

    mEdges[nbEdges++] = e;
    return &(mEdges[nbEdges - 1]);
}

// -----------------------------------------------------------------------------

void IcoSphere::build_topology(const Icosahedron& ico)
{
    *((loaders::Mesh*)this) = ico;

    mEdges.resize( 30 );
    nbEdges = 0;

    for(int i = 0; i < mNbTriangles; i++)
    {
        const TriangleIndex& tidx = mTriangles[i];
        Edge* e[3];

        for(int j = 0; j < 3; j++)
            e[j] = find_edge(tidx[j], tidx[(j+1)%3]);

        mTri.push_back( Tri(tidx[0], tidx[1], tidx[2], e[0], e[1], e[2]) );
    }
}

// -----------------------------------------------------------------------------

void IcoSphere::copy_result()
{
    // Drop edges
    erase_vec( mEdges );
    nbEdges = 0;

    // copy and drop triangles
    mNbTriangles = mTri.size();
    mTriangles.clear();
    mTriangles.resize( mNbTriangles );
    int acc = 0;
    std::list<Tri>::iterator it = mTri.begin();
    for(; it != mTri.end(); ++it)
    {
        const Tri& tr = *it;
        mTriangles[acc++] = TriangleIndex(tr[0], tr[1], tr[2]);
    }
    erase_list( mTri );
}

// -----------------------------------------------------------------------------

glm::vec2 IcoSphere::get_uv(const glm::vec3& pos) const
{
    float u;

    float v = acos(pos.z) / M_PI;
    if (pos.y >= 0) u = acos( pos.x / sin(M_PI*v) ) / (M_PI*2.f);
    else            u = (M_PI + acos(pos.x / sin(M_PI*v) )) / (M_PI*2.f);

    return glm::vec2( u, v );
}

// -----------------------------------------------------------------------------

unsigned IcoSphere::add_mid(Tri& tr,
                            int edgeIdx,
                            std::vector<Edge>& newEdges,
                            int& nbNewEdges)
{
    Edge* edge = tr.e[edgeIdx];
    if(edge->sub_e[0] != 0)
        return edge->mid;

    glm::vec3 v0 = mVertices[ edge->v[0] ].position;
    glm::vec3 v1 = mVertices[ edge->v[1] ].position;
    Vertex mid( glm::normalize((v0 + v1)*0.5f) );
    mid.normal   = mid.position;
    mid.texcoord = get_uv(mid.position);

    mVertices.push_back( mid );

    unsigned midIdx = mVertices.size() - 1;
    newEdges[nbNewEdges++] = Edge(edge->v[0], midIdx    );
    newEdges[nbNewEdges++] = Edge(midIdx    , edge->v[1]);

    edge->sub_e[0] = &(newEdges[ nbNewEdges - 2]);
    edge->sub_e[1] = &(newEdges[ nbNewEdges - 1]);

    edge->mid = midIdx;

    return midIdx;
}

// -----------------------------------------------------------------------------

IcoSphere::Edge* IcoSphere::find_sub_edge(const Edge* e, unsigned vidx) const
{
    assert(e != 0);
    assert(e->sub_e[0] != 0);
    assert(e->sub_e[1] != 0);

    Edge* sub_e0 = e->sub_e[0];
    if(sub_e0->v[0] == vidx || sub_e0->v[1] == vidx)
        return e->sub_e[0];

    Edge* sub_e1 = e->sub_e[1];
    if(sub_e1->v[0] == vidx || sub_e1->v[1] == vidx)
        return e->sub_e[1];

    return 0;
}


}// END rendersystem ===========================================================

#endif // ICOSAHEDRON_HPP__
