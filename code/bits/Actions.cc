#include "Actions.h"

#include <cassert>

#include <iostream>

#include "Character.h"
#include "Map.h"

namespace rCMI
{

  bool bump(Map &map, Character &character, gf::Vector2i target)
  {
    if (auto maybe_character = map.target_character_at(target); maybe_character)
      return melee(map, character, target);

    return movement(map, character, target);
  }

  bool movement(Map &map, Character &character, gf::Vector2i target)
  {
    if (map.blocking_entity_at(target))
      return false;

    if (!character.canWalkTo(target))
      return false;

    if (&character != &map.hero() && target == map.hero().getExistence().getPosition())
      return false;

    character.getExistence().setPosition(target);
    return true;
  }

  bool melee(Map &map, Character &character, gf::Vector2i target)
  {
    auto maybe_character = map.target_character_at(target);
    auto &other = map.getCharacters()[*maybe_character];

    int damage = character.getStat().getPower() - other.getStat().getDefense();
    std::string description = character.getExistence().getName() + " attacks " + other.getExistence().getName();

    if (damage > 0)
      other.take_damage(damage);

    return true;
  }

  bool shoot(Map &map, Character &character, gf::Vector2i target)
  {
    int distance = gf::chebyshevDistance(character.getExistence().getPosition(), target);

    if (distance > 3)
      return false;

    auto maybe_character = map.target_character_at(target);
    if (!maybe_character)
      return false;

    auto &other = map.getCharacters()[*maybe_character];

    int damage = character.getStat().getPower() - other.getStat().getDefense();

    std::cout << "Tir sur " << other.getExistence().getName() << " (" << damage << " dmg)\n";

    if (damage > 0)
      other.take_damage(damage);

    return true;
  }

}