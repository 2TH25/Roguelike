#ifndef ACTOR_H
#define ACTOR_H

#include <optional>

#include "HostileEntity.h"
#include "Player.h"
#include "PassiveEntity.h"



namespace rCMI {

   struct Actor {
    PassiveEntity passive;
    Player player;
    HostileEntity hostile;

    bool alive() const
    {
      return true;
    }

    void take_damage(int damage);
    void die();

    static Actor hero(gf::Vector2i position);
    static Actor orc(gf::Vector2i position);
    static Actor troll(gf::Vector2i position);

   };

}

#endif // ACTOR_H