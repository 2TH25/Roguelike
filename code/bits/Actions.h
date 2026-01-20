#ifndef ACTIONS_H
#define ACTIONS_H

#include <gf/Vector.h>

namespace rCMI
{
  class WorldEntity;
  class Character;

  bool bump(WorldEntity &m_world, Character &character, gf::Vector2i target);
  bool movement(WorldEntity &m_world, Character &character, gf::Vector2i target);
  bool melee(WorldEntity &m_world, Character &character, gf::Vector2i target);
  bool shoot(WorldEntity &m_world, Character &character, gf::Vector2i target);
}

#endif // ACTIONS_H