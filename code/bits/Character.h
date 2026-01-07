#ifndef CHARACTER_H
#define CHARACTER_H

#include <optional>
#include "Comportment.h"
#include "Stat.h"
#include "Entity.h"

namespace rCMI {
  class Character {
  public:

    Entity& getEntity() { return entity; }
    const Entity& getEntity() const { return entity; }
    
    Stat& getStat() { return stat; }
    const Stat& getStat() const { return stat; }
    Comportment& getComportment() { return comportment; }
    const Comportment& getComportment() const { return comportment; }

    bool alive() const {
      return comportment.getVariant().index() != 0 && stat.getHealth() > 0;
    }

    void take_damage(int damage);
    void die();
    

    static Character hero(gf::Vector2i position);
    static Character skeleton(gf::Vector2i position);
    static Character zombie(gf::Vector2i position);
    static Character slime(gf::Vector2i position);


    private: 
      Entity entity;
      Stat stat;
      Comportment comportment;
  };
}

#endif // CHARACTER_H