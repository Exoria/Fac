#ifndef TABLES_MCUBE_H
#define TABLES_MCUBE_H

// MARCHING CUBES ==============================================================
namespace Table
{
// Liste des arrête d'un cube intersecté par rapport à un index de 8bits
// 1bit à 1 de l'index de sommet indique qu'un sommet du cube est à l'extérieur
extern int edgeTable[256];

// Liste des configurations de triangles selon l'index de sommet
// La liste de 16 éléments indique l'ordre des vertex à relier trois par trois
extern int triTable[256][16];
}

#endif // TABLES_MCUBE_H
