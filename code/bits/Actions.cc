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

    character.getEntity().setPosition(target);
    return true;
  }

  bool melee(Map& map, Character& character, gf::Vector2i target) {
  auto maybe_character = map.target_character_at(target);
  auto& other = map.getCharacters()[*maybe_character]; // Utilise le getter de Map

  // Utilisation des getters
  int damage = character.getStat().getPower() - other.getStat().getDefense();
  std::string description = character.getEntity().getName() + " attacks " + other.getEntity().getName();

  if (damage > 0) {
    other.take_damage(damage);
  }
  return true;
}

}