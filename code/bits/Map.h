#ifndef MAP_H
#define MAP_H

#include <gf/TileLayer.h>
#include <gf/Texture.h>
#include <vector>
#include <gf/Random.h>
#include <optional>

#include "Tile.h"
#include "Character.h"
#include "RogueCMI.h"

namespace rCMI
{
  class Map : public gf::Entity
  {
  public:
    Map(RogueCMI *game);

    std::vector<Character> &getCharacters() { return characters; }
    gf::Vector2i getSize() const { return size; }
    void setSize(gf::Vector2i newSize) { size = newSize; }
    void setCharacters(const std::vector<Character> &newCharacters) { characters = newCharacters; }

    Character &hero() { return characters.front(); }

    std::optional<std::size_t> target_character_at(gf::Vector2i target);
    bool blocking_entity_at(gf::Vector2i target);

    std::vector<gf::Vector2i> compute_path(gf::Vector2i origin, gf::Vector2i target);
    void update_tile_at(gf::Vector2i pos, TileType type);

    void render(gf::RenderTarget &target, const gf::RenderStates &states);

    bool isVisible(gf::Vector2i position) const
    {
      return true;
    }

    bool isWalkable(gf::Vector2i position) const;

    static Map generate_board(gf::Vector2i size, RogueCMI *game);

  private:
    gf::Vector2i size;
    std::vector<Character> characters;
    gf::TileLayer tileLayer;
    RogueCMI *m_game = nullptr;
  };
};

#endif