#ifndef MAP_H
#define MAP_H

#include <gf/Tileset.h>
#include <gf/Grid.h>
#include <gf/Map.h>
#include <gf/Texture.h>
#include <vector>
#include <gf/Random.h>
#include <optional>

#include "Tile.h"
#include "Character.h"

namespace rCMI
{
  class RogueCMI;

  class Map : public gf::Entity
  {
  public:
    Map(RogueCMI *game);

    std::vector<Character> &getCharacters() { return characters; }
    gf::Vector2i getSize() const { return size; }

    Character &hero() { return characters.front(); }

    std::optional<std::size_t> target_character_at(gf::Vector2i target);
    bool blocking_entity_at(gf::Vector2i target);

    std::vector<gf::Vector2i> compute_path(gf::Vector2i origin, gf::Vector2i target);
    void update_tile_at(gf::Vector2i pos);
    void EnemyTurns();

    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

    bool isVisible(gf::Vector2i position) const { return true; }
    bool isWalkable(gf::Vector2i position) const;
    bool isStairs(gf::Vector2i position) const;
    void nextLevel();

    void generate_board();
    void generate_dungeon(gf::Vector2i Map_size);

    void clearMap();
    void fieldOfVision();

  private:
    gf::Vector2i size;
    std::vector<Character> characters;
    gf::SquareMap *grid = nullptr;
    RogueCMI *m_game = nullptr;
  };
};

#endif