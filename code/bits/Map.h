#ifndef MAP_H
#define MAP_H

#include <gf/TileLayer.h>
#include <gf/Texture.h>
#include <vector>
#include <gf/Random.h>
#include <optional>

#include "Tile.h"
#include "Character.h"

namespace rCMI {
  class Map : public gf::Entity {
    public:
    Map();
    std::vector<Character>& getCharacters() { return characters; }
    gf::Vector2i getSize() const { return size; }
    std::size_t getTilesetId() const { return tilesetId; }
    std::vector<rCMI::Tile> getTiles() const { return tiles; }
    void setSize(gf::Vector2i newSize) { size = newSize; }
    void setTilesetId(std::size_t id) { tilesetId = id; }
    void setTiles(const std::vector<rCMI::Tile>& newTiles) { tiles = newTiles; }
    void setCharacters(const std::vector<Character>& newCharacters) { characters = newCharacters; }

      Character& hero() { return characters.front(); }

      std::optional<std::size_t> target_character_at(gf::Vector2i target);
      bool blocking_entity_at(gf::Vector2i target);

      std::vector<gf::Vector2i> compute_path(gf::Vector2i origin, gf::Vector2i target);
      void update_tile_at(gf::Vector2i pos, TileType type);
      void render(gf::RenderTarget& renderer);
      bool isVisible(gf::Vector2i position) const {
        return true;
      }
      static Map generate_board(gf::Vector2i size,const gf::Texture& wallTex, const gf::Texture& floorTex);

    private:

    gf::Vector2i size;
    std::vector<rCMI::Tile> tiles;
    std::vector<Character> characters;
    std::size_t tilesetId;
    std::vector<TileType>grid;
    gf::TileLayer tileLayer;

  };

  

};

#endif