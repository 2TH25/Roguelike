#ifndef CHARACTER_H
#define CHARACTER_H

#include <optional>
#include "Comportment.h"
#include "Stat.h"
#include "Existence.h"
#include <gf/Sprite.h>
#include <gf/Texture.h>
#include <gf/RenderTarget.h>

namespace rCMI {
  class Character {
  public:

    Existence& getExistence() { return existence; }
    const Existence& getExistence() const { return existence; }
    
    Stat& getStat() { return stat; }
    const Stat& getStat() const { return stat; }
    Comportment& getComportment() { return comportment; }
    const Comportment& getComportment() const { return comportment; }

    bool alive() const {
      return comportment.getVariant().index() != 0 && stat.getHealth() > 0;
    }

    void take_damage(int damage);
    void die();
    void render(gf::RenderTarget& target, const gf::Texture& texture, int tileSize);
    

    static Character hero(gf::Vector2i position);
    static Character skeleton(gf::Vector2i position);
    static Character zombie(gf::Vector2i position);
    static Character slime(gf::Vector2i position);


    private: 
      Existence existence;
      Stat stat;
      Comportment comportment;
  };
}

#endif // CHARACTER_H