#include "Actions.h"

#include <cassert>

#include <iostream>

#include "Character.h"
#include "WorldEntity.h"

namespace rCMI
{

  bool bump(WorldEntity &m_world, Character &character, gf::Vector2i target)
  {
    if (auto maybe_character = m_world.target_character_at(target); maybe_character)
      return melee(m_world, character, target);

    return movement(m_world, character, target);
  }

  bool movement(WorldEntity &m_world, Character &character, gf::Vector2i target)
  {
    if (m_world.blocking_entity_at(target))
      return false;

    if (!character.canWalkTo(target))
      return false;

    if (&character != &m_world.hero() && target == m_world.hero().getExistence().getPosition())
      return false;

    character.getExistence().setPosition(target);
    return true;
  }

  bool melee(WorldEntity &m_world, Character &character, gf::Vector2i target)
  {
    auto maybe_character = m_world.target_character_at(target);
    auto &other = m_world.getCharacters()[*maybe_character];

    int damage = character.getStat().getPower();
    std::string description = character.getExistence().getName() + " attacks " + other.getExistence().getName();

    other.take_damage(damage);

    return true;
  }

  bool shoot(WorldEntity &m_world, Character &character, gf::Vector2i target)
  {
    int distance = gf::chebyshevDistance(character.getExistence().getPosition(), target);

    if (distance > 5)
      return false;

    auto maybe_character = m_world.target_character_at(target);
    if (!maybe_character)
      return false;

    auto &other = m_world.getCharacters()[*maybe_character];

    int damage = character.getStat().getPower();

    
    other.take_damage(damage);

    return true;
  }

}