#ifndef MAP_H
#define MAP_H

#include <vector>
#include <gf/Vector.h>
#include <gf/Tileset.h>
#include <gf/TileLayer.h>

#include "Tile.h"

class Map {
public:
    Map(gf::Vector2i size, gf::Vector2i tileSize);

    void render(gf::RenderTarget& target, gf::RenderStates states = gf::RenderStates());

private:
    gf::Vector2i m_mapSize;
    gf::Vector2i m_tileSize;
    gf::TileLayer m_layer;    // Le moteur de rendu de la grille
};

#endif