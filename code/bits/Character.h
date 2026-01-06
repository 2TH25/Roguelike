#ifndef CHARACTER_H
#define CHARACTER_H

#include <optional>

#include "Comportment.h"
#include "Stat.h"
#include "Entity.h"

namespace rCMI {

  struct Character {
    Entity entity;
    Stat stat;
    Comportment comportment;

    bool alive() const
    {
      return comportment.variant.index() != 0;
    }

    void take_damage(int damage);
    void die();

    static Character hero(gf::Vector2i position);
    static Character orc(gf::Vector2i position);
    static Character troll(gf::Vector2i position);

  };

}

#endif // CHARACTER_H