#ifndef CHARACTER_H
#define CHARACTER_H

#include "Comportment.h"
#include "Stat.h"
#include "Existence.h"
#include <gf/Entity.h>
#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <gf/Random.h>

namespace rCMI {
  class Character : public gf::Entity {
  public:
    Character() = default;
    Character(const Existence& ex, const Stat& st, const gf::Texture& tex);

    void setAppearance(const gf::Texture& tex) {
        texture = &tex;
    }
    void setDeadTexture(const gf::Texture& tex) {
        deadTexture = &tex;
    }

    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

    void doMove(Map& map);
    Existence& getExistence() { return existence; }
    Stat& getStat() { return stat; }
    Comportment& getComportment() { return comportment; }
    void setExistence(const Existence& ex) { existence = ex; }
    void setStat(const Stat& st) { stat = st; }
    void setComportment(const Comportment& comp) { comportment = comp; }


    bool alive() const {
      return stat.getHealth() > 0;
    }

    void take_damage(int damage);
    void die();

    void goUp(Map& map);
    void goDown(Map& map);
    void goLeft(Map& map);
    void goRight(Map& map);

    static Character hero(gf::Vector2i position, const gf::Texture& tex);
    static Character skeleton(gf::Vector2i position, const gf::Texture& tex);
    static Character zombie(gf::Vector2i position, const gf::Texture& tex);
    static Character slime(gf::Vector2i position, const gf::Texture& tex);

    private: 
      Existence existence;
      Stat stat;
      Comportment comportment;
      const gf::Texture* texture = nullptr;
      const gf::Texture* deadTexture = nullptr;
  };
}
#endif // CHARACTER_H

