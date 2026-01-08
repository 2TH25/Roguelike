#ifndef CHARACTER_H
#define CHARACTER_H

#include "Comportment.h"
#include "Stat.h"
#include "Existence.h"
#include <gf/Entity.h>
#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
namespace rCMI {
  class Character : public gf::Entity {
  public:
    Character() = default;
    Character(const Existence& ex, const Stat& st, const gf::Texture& tex);

    void setAppearance(const gf::Texture& tex) {
        texture = &tex;
    }

    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

    Existence& getExistence() { return existence; }
    Stat& getStat() { return stat; }
    Comportment& getComportment() { return comportment; }
    void setExistence(const Existence& ex) { existence = ex; }
    void setStat(const Stat& st) { stat = st; }
    void setComportment(const Comportment& comp) { comportment = comp; }


    bool alive() const {
      return comportment.getVariant().index() != 0 && stat.getHealth() > 0;
    }

    void take_damage(int damage);
    void die();

    void goUp(const Map& map);
    void goDown(const Map& map);
    void goLeft(const Map& map);
    void goRight(const Map& map);

    static Character hero(gf::Vector2i position, const gf::Texture& tex);
    static Character skeleton(gf::Vector2i position, const gf::Texture& tex);
    static Character zombie(gf::Vector2i position, const gf::Texture& tex);
    static Character slime(gf::Vector2i position, const gf::Texture& tex);

    private: 
      Existence existence;
      Stat stat;
      Comportment comportment;
      const gf::Texture* texture = nullptr;
  };
}
#endif // CHARACTER_H

