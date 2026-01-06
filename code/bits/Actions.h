#ifndef ACTIONS_H
#define ACTIONS_H

#include <gf/Vector.h>

namespace rCMI {
  struct Map;
  struct Actor;

  bool bump(Map& map, Actor& actor, gf::Vector2i target);
  bool movement(Map& map, Actor& actor, gf::Vector2i target);
  bool melee(Map& map, Actor& actor, gf::Vector2i target);


}

#endif