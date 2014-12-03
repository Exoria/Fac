#ifndef TABLES_MCUBE_H
#define TABLES_MCUBE_H

// MARCHING CUBES ==============================================================
namespace Table
{
// Liste des arr�te d'un cube intersect� par rapport � un index de 8bits
// 1bit � 1 de l'index de sommet indique qu'un sommet du cube est � l'ext�rieur
extern int edgeTable[256];

// Liste des configurations de triangles selon l'index de sommet
// La liste de 16 �l�ments indique l'ordre des vertex � relier trois par trois
extern int triTable[256][16];
}

#endif // TABLES_MCUBE_H
