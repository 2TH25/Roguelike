#include "WorldEntity.h"

#include "RogueCMI.h"
#include "Map.h"
#include "bsp.h"

#include <queue>
#include <map>
#include <cassert>
#include <optional>
#include <algorithm>
#include <gf/Geometry.h>
#include <gf/TileLayer.h>
#include <random>
#include <iostream>
#include <gf/Particles.h>

namespace rCMI
{

  WorldEntity::WorldEntity(RogueCMI *game)
      : gf::Entity(0),
        m_game(game),
        m_map(game)
  {
  }

  void WorldEntity::update_tile_at(gf::Vector2i pos, TileType type) { m_map.update_tile_at(pos, type); }

  void WorldEntity::update(gf::Time time)
  {
    for (auto &character : characters)
    {
      character.update(time);
    }
  }

  void WorldEntity::generate_board()
  {
    m_map.generate_board();

    characters.clear();
    Character hero = Character::hero({5, 5}, m_game->resources.getTexture("perso640/SetPerso.png"));
    const gf::Texture &textureMort = m_game->resources.getTexture("mort.png");
    hero.setDeadTexture(textureMort);
    characters.push_back(hero);

    characters.back().playAnimation("Default");

    // characters.push_back(Character::slime({5, 4}, m_game->resources.getTexture("slime.png")));
    characters.push_back(Character::zombie({2, 6}, m_game->resources.getTexture("zombie.png")));
    // characters.push_back(Character::skeleton({1, 8}, m_game->resources.getTexture("squelette.png")));
  }

  std::optional<std::size_t> WorldEntity::target_character_at(gf::Vector2i target)
  {
    for (std::size_t i = 0; i < characters.size(); ++i)
      if (characters[i].getExistence().getPosition() == target && characters[i].alive())
        return i;

    return std::nullopt;
  }

  Character &WorldEntity::hero()
  {
    if (characters.empty())
      throw std::runtime_error("Tentative d'accès au Hero alors que la liste est vide !");

    return characters.front();
  }

  bool WorldEntity::blocking_entity_at(gf::Vector2i target) { return m_map.blocking_entity_at(target) ? true : target_character_at(target).has_value(); }

  bool WorldEntity::isWalkable(gf::Vector2i position) const { return m_map.isWalkable(position); }

  void WorldEntity::nextLevel()
  {
    m_chestManager.clear();
    generate_dungeon(m_map.getSize() * 1.2);
    highest_level++;
    std::cout << "Niveau suivant atteint !" << std::endl;
  }

  bool WorldEntity::isStairs(gf::Vector2i position) const { return m_map.isStairs(position); }

  bool WorldEntity::isHealing(gf::Vector2i position) const { return m_map.isHealing(position); }

  bool WorldEntity::usHealing(gf::Vector2i position) { return m_map.usHealing(position); }

  void WorldEntity::EnemyTurns()
  {
    for (std::size_t i = 1; i < characters.size(); ++i)
      characters[i].doMove(*this);
  }

  void WorldEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    m_map.render(target, states);

    if (!m_map.isMiniMap())
    {
      m_chestManager.render(target, states, m_map);
      for (std::size_t i = 0; i < characters.size(); ++i)
        if ((characters[i].alive() || i == 0) && m_map.isInFieldOfVision(characters[i].getExistence().getPosition()))
          characters[i].render(target, states);
    }
    else
    {
      gf::RectangleShape player_chape(gf::RectF::fromPositionSize(hero().getExistence().getPosition() * TileSize, {TileSize, TileSize}));
      player_chape.setColor(gf::Color::fromRgba32(234, 51, 35));
      player_chape.draw(target, states);
    }
  }

  void WorldEntity::generate_dungeon(gf::Vector2i Map_size)
  {

    std::optional<Stat> savedStats = std::nullopt;

    if (!characters.empty())
    {
      savedStats = characters[0].getStat();
    }
    m_map.generate_dungeon(Map_size);

    rCMI::BSP generator;
    Dungeon dungeon = generator.generate(m_game->random, Map_size);

    for (int y = 0; y < Map_size.y; ++y)
      for (int x = 0; x < Map_size.x; ++x)
        update_tile_at({x, y}, dungeon.getTile({x, y}));

    characters.clear();
    m_chestManager.m_chests.clear();
    characters.reserve(100);
    std::vector<BSPTree *> leaves;
    generator.getRoot().getRooms(leaves);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist_type(1, 100);

    for (auto *leaf : leaves)
    {
      if (leaf->type == RoomType::Start)
      {
        gf::Vector2i center = leaf->room.getCenter();
        m_map.setTileTexture(center, {1, 2});

        Character hero = Character::hero(center, m_game->resources.getTexture("perso640/SetPerso.png"));
        const gf::Texture &textureMort = m_game->resources.getTexture("mort.png");
        hero.setDeadTexture(textureMort);
        if (savedStats.has_value())
        {
          hero.setStat(savedStats.value());
        }
        characters.push_back(hero);
        characters.back().playAnimation("Default");

        gf::Vector2i pnjPos = center + gf::Vector2i(1, 1);
    
        const gf::Texture &texPNJ = m_game->resources.getTexture("Fée.png"); 
        characters.push_back(Character::pnj(pnjPos, texPNJ));

        break;
      }
    }

    for (auto *leaf : leaves)
    {
      gf::RectI room = leaf->room;
      gf::Vector2i center = room.getCenter();

      std::vector<gf::Vector2i> floorPositions;
      for (int x = room.min.x + 1; x < room.max.x; ++x)
      {
        for (int y = room.min.y + 1; y < room.max.y; ++y)
        {
          floorPositions.push_back({x, y});
        }
      }
      std::shuffle(floorPositions.begin(), floorPositions.end(), gen);

      if (leaf->type == RoomType::Chest)
      {
        std::cout << "Génération d'une salle aux trésors en " << room.min.x << "," << room.min.y << std::endl;
        int chestCount = 4;
        for (int i = 0; i < chestCount && i < floorPositions.size(); ++i)
        {
          m_chestManager.spawnChest(floorPositions[i], m_game);
        }
      }
      if (leaf->type == RoomType::End)
      {
        update_tile_at(center, TileType::Stairs);
      }

      else if (leaf->type == RoomType::Healing)
      {
        int healCount = 3;
        for (int i = 0; i < healCount && i < floorPositions.size(); ++i)
        {
          update_tile_at(floorPositions[i], TileType::HealingFloor);
        }
      }
      else if (leaf->type == RoomType::Start)
      {
        continue;
      }
      else
      {
        std::uniform_int_distribution<> dist_nb_mob(0, 2);
        int nbMobs = dist_nb_mob(gen);

        std::uniform_int_distribution<> dist_room_x(room.min.x + 1, room.max.x - 1);
        std::uniform_int_distribution<> dist_room_y(room.min.y + 1, room.max.y - 1);

        for (int i = 0; i < nbMobs; i++)
        {
          gf::Vector2i pos_aleatoire;
          bool validPos = false;
          int attempts = 0;

          do
          {
            pos_aleatoire.x = dist_room_x(gen);
            pos_aleatoire.y = dist_room_y(gen);
            if (isWalkable(pos_aleatoire) && !target_character_at(pos_aleatoire).has_value())
            {
              validPos = true;
            }
            attempts++;
          } while (!validPos && attempts < 10);

          if (validPos)
          {
            Character mob;
            int nombre_aleatoire = dist_type(gen);

            const gf::Texture &textureMonstres = m_game->resources.getTexture("SetTextureMonstre.png");

            if (nombre_aleatoire <= 60)
            {
              mob = Character::slime(pos_aleatoire, textureMonstres);
            }
            else if (nombre_aleatoire <= 85)
            {
              mob = Character::zombie(pos_aleatoire, textureMonstres);
            }
            else
            {
              mob = Character::skeleton(pos_aleatoire, textureMonstres);
            }

            mob.setHomeRoom(room);
            characters.push_back(mob);
          }
        }
      }
    }
  }

  struct VectorCompare
  {
    bool operator()(const gf::Vector2i &a, const gf::Vector2i &b) const
    {
      if (a.x != b.x)
        return a.x < b.x;
      return a.y < b.y;
    }
  };

  std::vector<gf::Vector2i> WorldEntity::compute_path(gf::Vector2i origin, gf::Vector2i target)
  {
    std::queue<gf::Vector2i> frontier;
    frontier.push(origin);

    std::map<gf::Vector2i, gf::Vector2i, VectorCompare> came_from;
    came_from[origin] = origin;
    gf::Vector2i directions[] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    while (!frontier.empty())
    {
      gf::Vector2i current = frontier.front();
      frontier.pop();
      if (current == target)
      {
        break;
      }

      for (auto &dir : directions)
      {
        gf::Vector2i next = current + dir;

        if (came_from.find(next) == came_from.end())
        {

          if (isWalkable(next) || next == target)
          {
            frontier.push(next);
            came_from[next] = current;
          }
        }
      }
    }
    std::vector<gf::Vector2i> path;

    if (came_from.find(target) == came_from.end())
    {
      return path;
    }

    gf::Vector2i current = target;
    while (current != origin)
    {
      path.push_back(current);
      current = came_from[current];
    }
    std::reverse(path.begin(), path.end());

    return path;
  }

  void WorldEntity::clearMap() { m_map.clearFieldOfVision(); }

  void WorldEntity::fieldOfVision() { m_map.computeFieldOfVision(hero().getExistence().getPosition()); }

  HudEntity::HudEntity(RogueCMI *m_game, WorldEntity *m_world)
      : Entity(1),
        m_game(m_game),
        m_world(m_world)
  {
    life_lost.setColor(gf::Color::Gray());
    life.setColor(gf::Color::Red);
    text_life.setFont(m_game->resources.getFont("DejaVuSans.ttf"));
    text_life.setColor(gf::Color::Black);

    xp_lost.setColor(gf::Color::Gray());
    xp.setColor(gf::Color::Green);
    text_xp.setFont(m_game->resources.getFont("DejaVuSans.ttf"));
    text_xp.setColor(gf::Color::Black);

    player_kills_image.setTexture(m_game->resources.getTexture("squelette.png"));
    text_kills.setFont(m_game->resources.getFont("DejaVuSans.ttf"));
    text_kills.setColor(gf::Color::Red);

    sword_slot.setTexture(m_game->resources.getTexture("SlotArme.png"));
  }

  void HudEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {

    if (m_world->getCharacters().empty())
      return;

    const gf::Vector2f target_vue_size(target.getView().getSize());

    life_lost.setPosition({target_vue_size.y * 5 / 100, target_vue_size.y * 5 / 100});
    life_lost.setSize({target_vue_size.x * 10 / 49, target_vue_size.y * 20 / 817});
    life_lost.setAnchor(gf::Anchor::CenterLeft);

    life.setPosition(life_lost.getPosition());
    float life_to_size = target_vue_size.x * 10 / 49 * m_world->hero().getStat().getHealth() / m_world->hero().getStat().getMaxHealth();
    life.setSize({life_to_size, target_vue_size.y * 20 / 817});
    life.setAnchor(gf::Anchor::CenterLeft);

    text_life.setString(std::to_string(m_world->hero().getStat().getHealth()) + " / " + std::to_string(m_world->hero().getStat().getMaxHealth()));
    text_life.setCharacterSize(life_lost.getSize().y * 15 / 20);
    text_life.setPosition({life_lost.getPosition().x + life_lost.getSize().x / 2, life_lost.getPosition().y});
    text_life.setAnchor(gf::Anchor::Center);

    xp_lost.setPosition({target_vue_size.y * 5 / 100, target_vue_size.y * 8 / 100});
    xp_lost.setSize({target_vue_size.x * 10 / 49, target_vue_size.y * 20 / 817});
    xp_lost.setAnchor(gf::Anchor::CenterLeft);

    xp.setPosition(xp_lost.getPosition());
    float xp_to_size = xp_lost.getSize().x * m_world->hero().getStat().getXp() / m_world->hero().getStat().getMaxXp();
    xp.setSize({xp_to_size, xp_lost.getSize().y});
    xp.setAnchor(gf::Anchor::CenterLeft);

    text_xp.setString(std::to_string(m_world->hero().getStat().getLevel()));
    text_xp.setCharacterSize(xp_lost.getSize().y * 15 / 20);
    text_xp.setPosition({xp_lost.getPosition().x + xp_lost.getSize().x / 2, xp_lost.getPosition().y});
    text_xp.setAnchor(gf::Anchor::Center);

    player_kills_image.setPosition({target_vue_size.x * 94 / 100, target_vue_size.y * 5 / 100});
    player_kills_image.setScale(life.getSize().y * 3 / player_kills_image.getTexture().getSize());
    player_kills_image.setAnchor(gf::Anchor::Center);

    text_kills.setString(std::to_string(m_world->hero().getStat().getKills()));
    text_kills.setCharacterSize(life_lost.getSize().y);
    text_kills.setPosition({target_vue_size.x * 96 / 100, player_kills_image.getPosition().y});
    text_kills.setAnchor(gf::Anchor::CenterLeft);

    sword_slot.setPosition({life_lost.getPosition().x, target_vue_size.y - life_lost.getPosition().x});
    sword_slot.setScale(life.getSize().y * 5 / sword_slot.getTexture().getSize());
    sword_slot.setAnchor(gf::Anchor::BottomLeft);

    Item weapon = m_game->m_InventoryScene.get()->m_inventory.getEquippedItem(ItemType::Weapon);
    sword.setTexture(*weapon.m_texture);
    sword.setPosition({life_lost.getPosition().x, target_vue_size.y - life_lost.getPosition().x});
    sword.setScale(life.getSize().y * 5 / sword_slot.getTexture().getSize());
    sword.setAnchor(gf::Anchor::BottomLeft);

    life_lost.draw(target, states);
    life.draw(target, states);
    text_life.draw(target, states);

    xp_lost.draw(target, states);
    xp.draw(target, states);
    text_xp.draw(target, states);

    player_kills_image.draw(target, states);
    text_kills.draw(target, states);

    sword_slot.draw(target, states);
    sword.draw(target, states);
  }

  void WorldEntity::reset()
  {
    highest_level = 1;
    m_chestManager.clear();
    characters.clear();
    generate_dungeon(MapSize);
    clearMap();
    fieldOfVision();

    std::cout << "WorldEntity réinitialisée : Nouvelle partie lancée." << std::endl;
  }

  void WorldEntity::activateMiniMap()
  {
    m_map.activateMiniMap();
  }
}