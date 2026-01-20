#ifndef WORLD_ENTITY_H
#define WORLD_ENTITY_H

#include <gf/Tileset.h>
#include <gf/Grid.h>
#include <gf/Texture.h>
#include <vector>
#include <gf/Random.h>
#include <optional>
#include "Character.h"
#include "Map.h"
#include "ItemManager.h"

namespace rCMI
{
  class RogueCMI;

  class WorldEntity : public gf::Entity
  {
  public:
    WorldEntity(RogueCMI *game);
    ItemManager m_itemManager;

    std::vector<Character> &getCharacters() { return characters; }

    Character &hero() { return characters.front(); }

    std::optional<std::size_t> target_character_at(gf::Vector2i target);
    bool blocking_entity_at(gf::Vector2i target);

    std::vector<gf::Vector2i> compute_path(gf::Vector2i origin, gf::Vector2i target);
    void update_tile_at(gf::Vector2i pos, TileType type);
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

    Map &getMap(){ return m_map; };

  private:
    RogueCMI *m_game = nullptr;
    std::vector<Character> characters;
    Map m_map;
    
  };
};

#endif