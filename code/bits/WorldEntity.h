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
#include "ChestManager.h"
#include "Inventory.h"
#include <gf/Text.h>

namespace rCMI
{
  class RogueCMI;

  class WorldEntity : public gf::Entity
  {
  public:
    WorldEntity(RogueCMI *game);
    ChestManager m_chestManager;

    std::vector<Character> &getCharacters() { return characters; }

    Character &hero();

    std::optional<std::size_t> target_character_at(gf::Vector2i target);
    bool blocking_entity_at(gf::Vector2i target);

    std::vector<gf::Vector2i> compute_path(gf::Vector2i origin, gf::Vector2i target);
    void update_tile_at(gf::Vector2i pos, TileType type);
    void update(gf::Time time) override;
    void toggleMinimapMode(bool active);
    void EnemyTurns();

    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

    bool isVisible(gf::Vector2i position) const { return true; }
    bool isWalkable(gf::Vector2i position) const;
    bool isStairs(gf::Vector2i position) const;
    bool isHealing(gf::Vector2i position) const;
    bool usHealing(gf::Vector2i position);
    void nextLevel();

    void generate_board();
    void generate_dungeon(gf::Vector2i Map_size);

    void clearMap();
    void fieldOfVision();

    void reset();

    Map &getMap() { return m_map; };

    int highest_level = 1;
    int kills = 12;
    int score = 200;
  private:
    RogueCMI *m_game = nullptr;
    std::vector<Character> characters;
    Map m_map;
    std::map<Character*, const gf::Texture*> m_savedTextures;
  };

  class HudEntity : public gf::Entity
  {
  public:
    HudEntity(RogueCMI *m_game, WorldEntity *m_world);
    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    RogueCMI *m_game = nullptr;
    WorldEntity *m_world = nullptr;

    gf::RectangleShape life_lost;
    gf::RectangleShape life;
    gf::Text text_life;
    
    gf::RectangleShape xp_lost;
    gf::RectangleShape xp;
    gf::Text text_xp;

    gf::Sprite player_kills_image;
    gf::Text text_kills;

    gf::Sprite sword_slot;
    gf::Sprite sword;
  };
};

#endif