#ifndef ACTIONS_H
#define ACTIONS_H

#include <gf/Vector.h>

namespace rCMI { // Changé de rl à rCMI pour la cohérence
  class Map;
  class Character;

  bool bump(Map& map, Character& character, gf::Vector2i target);
  bool movement(Map& map, Character& character, gf::Vector2i target);
  bool melee(Map& map, Character& character, gf::Vector2i target);
}

#endif // ACTIONS_H