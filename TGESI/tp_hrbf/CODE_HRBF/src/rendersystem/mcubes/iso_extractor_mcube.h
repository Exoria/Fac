#ifndef MARCHING_CUBE_H
#define MARCHING_CUBE_H

#include <cmath>
#include "tables_mcube.h"
#include "../hrbf/bbox.h"

//=============================================================================
namespace MCube {
//=============================================================================


    struct Triangle {
        BBox::Point p[3];
    };

    typedef struct {
        BBox::Point p[8]; // Coordonnées des 8 sommets d'un cube
        float val[8]; // Iso-valeur en ces 8 sommets
    } GridCell;

    /*
       Given a grid grid and an isolevel, calculate the triangular
       facets required to represent the isosurface through the cell.
       Return the number of triangular facets, the array "triangles"
       will be loaded up with the vertices at most 5 triangular facets.
            0 will be returned if the grid cell is either totally above
       of totally below the isolevel.
    */
    int polygonise(const GridCell& grid,
                   float isolevel,
                   Triangle triangles[5]);

    /// @return un point par interpolation linéaire entre p0 et p1
    /// selon les poids valp1 et valp2
    BBox::Point vertexInterp(float isolevel,
                             const BBox::Point& p1,
                             const BBox::Point& p2,
                             float valp1,
                             float valp2);
}
//=============================================================================

#endif // MARCHING_CUBE_H
