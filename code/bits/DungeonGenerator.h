#ifndef DUNGEON_GENERATOR_H
#define DUNGEON_GENERATOR_H

#include <gf/Array2D.h>
#include <gf/Random.h>
#include <gf/Vector.h>
#include "Tile.h"

namespace rCMI {

  struct Dungeon {
    gf::Vector2i size;
    std::vector<TileType> grid; // Le tableau qui stocke les murs et les sols

    // Constructeur par défaut
    Dungeon() = default;

    // Constructeur qui initialise la grille avec un type par défaut (ex: Wall)
    Dungeon(gf::Vector2i size, TileType type)
    : size(size)
    , grid(size.x * size.y, type)
    {
    }

    // La méthode pour récupérer une tuile en sécurité
    TileType getTile(gf::Vector2i pos) const {
      if (pos.x >= 0 && pos.x < size.x && pos.y >= 0 && pos.y < size.y) {
        return grid[pos.y * size.x + pos.x];
      }
      return TileType::Wall; // Par défaut si hors limite
    }

    // La méthode pour modifier une tuile
    void setTile(gf::Vector2i pos, TileType type) {
      if (pos.x >= 0 && pos.x < size.x && pos.y >= 0 && pos.y < size.y) {
        grid[pos.y * size.x + pos.x] = type;
      }
    }
  };

  class DungeonGenerator {
  public:
    enum class Phase {
      Start,
      Iterate,
      Finish,
    };

    DungeonGenerator();
    virtual ~DungeonGenerator();

    Phase getPhase() const {
      return m_phase;
    }

    void setPhase(Phase phase) {
      m_phase = phase;
    }

    virtual Dungeon generate(gf::Random &random, gf::Vector2i sizeMap) = 0;

  private:
    Phase m_phase;
  };

}


#endif