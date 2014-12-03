#include "iso_extractor_mcube.h"

int MCube::polygonise(const GridCell& grid, float isolevel, Triangle *triangles)
{
    int   ntriang;
    int   cubeindex;
    BBox::Point vertlist[12];

    /*
       Determine the index into the edge table which
       tells us which vertices are inside of the surface
    */
    cubeindex = 0;
    if (grid.val[0] < isolevel) cubeindex |= 1;
    if (grid.val[1] < isolevel) cubeindex |= 2;
    if (grid.val[2] < isolevel) cubeindex |= 4;
    if (grid.val[3] < isolevel) cubeindex |= 8;
    if (grid.val[4] < isolevel) cubeindex |= 16;
    if (grid.val[5] < isolevel) cubeindex |= 32;
    if (grid.val[6] < isolevel) cubeindex |= 64;
    if (grid.val[7] < isolevel) cubeindex |= 128;

    /* Cube is entirely in/out of the surface */
    if (Table::edgeTable[cubeindex] == 0)
        return(0);

    /* Find the vertices where the surface intersects the cube */
    if (Table::edgeTable[cubeindex] & 1)
        vertlist[0] =
            vertexInterp(isolevel,grid.p[0],grid.p[1],grid.val[0],grid.val[1]);
    if (Table::edgeTable[cubeindex] & 2)
        vertlist[1] =
            vertexInterp(isolevel,grid.p[1],grid.p[2],grid.val[1],grid.val[2]);
    if (Table::edgeTable[cubeindex] & 4)
        vertlist[2] =
            vertexInterp(isolevel,grid.p[2],grid.p[3],grid.val[2],grid.val[3]);
    if (Table::edgeTable[cubeindex] & 8)
        vertlist[3] =
            vertexInterp(isolevel,grid.p[3],grid.p[0],grid.val[3],grid.val[0]);
    if (Table::edgeTable[cubeindex] & 16)
        vertlist[4] =
            vertexInterp(isolevel,grid.p[4],grid.p[5],grid.val[4],grid.val[5]);
    if (Table::edgeTable[cubeindex] & 32)
        vertlist[5] =
            vertexInterp(isolevel,grid.p[5],grid.p[6],grid.val[5],grid.val[6]);
    if (Table::edgeTable[cubeindex] & 64)
        vertlist[6] =
            vertexInterp(isolevel,grid.p[6],grid.p[7],grid.val[6],grid.val[7]);
    if (Table::edgeTable[cubeindex] & 128)
        vertlist[7] =
            vertexInterp(isolevel,grid.p[7],grid.p[4],grid.val[7],grid.val[4]);
    if (Table::edgeTable[cubeindex] & 256)
        vertlist[8] =
            vertexInterp(isolevel,grid.p[0],grid.p[4],grid.val[0],grid.val[4]);
    if (Table::edgeTable[cubeindex] & 512)
        vertlist[9] =
            vertexInterp(isolevel,grid.p[1],grid.p[5],grid.val[1],grid.val[5]);
    if (Table::edgeTable[cubeindex] & 1024)
        vertlist[10] =
            vertexInterp(isolevel,grid.p[2],grid.p[6],grid.val[2],grid.val[6]);
    if (Table::edgeTable[cubeindex] & 2048)
        vertlist[11] =
            vertexInterp(isolevel,grid.p[3],grid.p[7],grid.val[3],grid.val[7]);

    /* Create the triangle */
    ntriang = 0;
    for (int i=0;Table::triTable[cubeindex][i]!=-1;i += 3)
    {
        triangles[ntriang].p[0] = vertlist[Table::triTable[cubeindex][i  ]];
        triangles[ntriang].p[1] = vertlist[Table::triTable[cubeindex][i+1]];
        triangles[ntriang].p[2] = vertlist[Table::triTable[cubeindex][i+2]];
        ntriang++;
    }

    return ntriang;
}

//=============================================================================

BBox::Point MCube::
vertexInterp(float isolevel,
             const BBox::Point& p1, const BBox::Point& p2,
             float valp1, float valp2)
{
    BBox::Point p;

    if (fabs(isolevel-valp1) < 0.00001)
        return p1;

    if (fabs(isolevel-valp2) < 0.00001)
        return p2;

    if (fabs(valp1-valp2) < 0.00001)
        return p1;

    p.x = p1.x + ((isolevel - valp1) / (valp2 - valp1)) * (p2.x - p1.x);
    p.y = p1.y + ((isolevel - valp1) / (valp2 - valp1)) * (p2.y - p1.y);
    p.z = p1.z + ((isolevel - valp1) / (valp2 - valp1)) * (p2.z - p1.z);

    return p;
}
