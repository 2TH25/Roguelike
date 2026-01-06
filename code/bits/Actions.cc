#include "Actions.h"

#include <cassert>

#include <iostream>

#include "Character.h"
#include "Map.h"

namespace rCMI {

  bool bump(Map& map, Character& character, gf::Vector2i target)
  {
    if (auto maybe_character = map.target_character_at(target); maybe_character) {
      return melee(map, character, target);
    }

    return movement(map, character, target);
  }

  bool movement(Map& map, Character& character, gf::Vector2i target)
  {

    if (map.blocking_entity_at(target)) {
      return false;
    }

    character.entity.position = target;
    return true;
  }

  bool melee(Map& map, Character& character, gf::Vector2i target)
  {
    auto maybe_character = map.target_character_at(target);
    assert(maybe_character);

    auto& other = map.characters[*maybe_character];

    int damage = character.stat.power - other.stat.defense;

    std::string description = character.entity.name + " attacks " + other.entity.name;
    if (damage > 0) {
      std::cout << description << " for " << damage << " hit points.\n";
      other.stat.health -= damage;
    } else {
      std::cout << description << " but does no damage.\n";
    }

    return true;
  }

}