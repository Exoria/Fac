#ifndef BBOX_CU_HPP__
#define BBOX_CU_HPP__

#include <float.h>
#include <vector>
#include <cassert>
#include <climits>

/**
  @struct BBox_cu
  @brief Representation with two 3d points of an axis aligned bounded box
*/
struct BBox{

    struct Point {

        Point(float x, float y, float z){
            this->x = x; this->y = y; this->z = z;
        }

        Point(){
            this->x = 0.f; this->y = 0.f; this->z = 0.f;
        }

        float x, y, z;
    };

    inline BBox():
        pmin(Point( FLT_MAX,  FLT_MAX,  FLT_MAX)),
        pmax(Point(-FLT_MAX, -FLT_MAX, -FLT_MAX))
    {
        // using infinity allows maximum and minimum to take the other value,
        // always. The union of two empty bboxes is still an empty bbox,
        // and the intersection of an empty bbox with another bbox
        // is still empty finally a point will be considered outside the bbox
    }


    inline BBox(const Point& a, const Point& b):
        pmin(Point(fminf(a.x, b.x), fminf(a.y, b.y), fminf(a.z, b.z))),
        pmax(Point(fmaxf(a.x, b.x), fmaxf(a.y, b.y), fmaxf(a.z, b.z)))
    {    }


    inline BBox(float xmin, float ymin, float zmin,
                float xmax, float ymax, float zmax):
        pmin(Point(xmin, ymin, zmin)),
        pmax(Point(xmax, ymax, zmax))
    {   }


    inline bool inside(const Point& p) const;

    inline BBox bbox_union(const BBox& bb) const;

    inline BBox bbox_isect(const BBox& bb) const;

    inline void add_point(const Point& p);


    /// Get the ith corner position. Indexation is not the same as in
    /// 'get_bbox_corners()'
    /**
        @code

            6 +----+ 7
             /|   /|
          2 +----+3|
            |4+--|-+5
            |/   |/
            +----+
           0      1
        // Vertex 0 is pmin and vertex 7 pmax
        @endcode
    */

    inline Point get_corner(int i) const;


    Point pmin;
    Point pmax;
};
// =============================================================================


inline bool BBox::inside(const Point& p) const {
    return (p.x >= pmin.x) & (p.y >= pmin.y) & (p.z >= pmin.z) &
            (p.x <= pmax.x) & (p.y <= pmax.y) & (p.z <= pmax.z);
}

// -----------------------------------------------------------------------------

inline
BBox BBox::bbox_union(const BBox& bb) const {
    return BBox(fminf(pmin.x, bb.pmin.x), fminf(pmin.y, bb.pmin.y), fminf(pmin.z, bb.pmin.z),
                fmaxf(pmax.x, bb.pmax.x), fmaxf(pmax.y, bb.pmax.y), fmaxf(pmax.z, bb.pmax.z));
}

// -----------------------------------------------------------------------------

inline
BBox BBox::bbox_isect(const BBox& bb) const {
    BBox res =
            BBox(fmaxf(pmin.x, bb.pmin.x), fmaxf(pmin.y, bb.pmin.y), fmaxf(pmin.z, bb.pmin.z),
                 fminf(pmax.x, bb.pmax.x), fminf(pmax.y, bb.pmax.y), fminf(pmax.z, bb.pmax.z));
    if((res.pmin.x > res.pmax.x) |
            (res.pmin.y > res.pmax.y) |
            (res.pmin.z > res.pmax.z)
            )
    {
        res = BBox();
    }
    return res;
}

// -----------------------------------------------------------------------------

inline
void BBox::add_point(const Point& p) {
    pmin.x = fminf(p.x, pmin.x);
    pmin.y = fminf(p.y, pmin.y);
    pmin.z = fminf(p.z, pmin.z);
    pmax.x = fmaxf(p.x, pmax.x);
    pmax.y = fmaxf(p.y, pmax.y);
    pmax.z = fmaxf(p.z, pmax.z);
}

// -----------------------------------------------------------------------------

inline BBox::Point BBox::get_corner(int i) const
{
    float diffx = pmax.x - pmin.x;
    float diffy = pmax.y - pmin.y;
    float diffz = pmax.z - pmin.z;
    diffx *= (i      & 0x1);
    diffy *= (i >> 1 & 0x1);
    diffz *= (i >> 2 & 0x1);
    return Point( pmin.x + diffx, pmin.y + diffy, pmin.z + diffz);
}

// -----------------------------------------------------------------------------

#endif
