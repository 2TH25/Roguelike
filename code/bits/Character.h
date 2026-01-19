#ifndef CHARACTER_H
#define CHARACTER_H

#include "Comportment.h"
#include "Stat.h"
#include "Existence.h"
#include <gf/Entity.h>
#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <gf/Random.h>

namespace rCMI
{
  class Character : public gf::Entity
  {
  public:
    Character() = default;
    Character(const Existence &ex, const Stat &st, const gf::Texture &tex);
    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

    void doMove(Map &map);
    void take_damage(int damage);
    void die();

    bool goUp(Map &map);
    bool goDown(Map &map);
    bool goLeft(Map &map);
    bool goRight(Map &map);

    Existence &getExistence() { return existence; }
    Stat &getStat() { return stat; }
    Comportment &getComportment() { return comportment; }

    void setAppearance(const gf::Texture &tex) { texture = &tex; }
    void setDeadTexture(const gf::Texture &tex) { deadTexture = &tex; }
    void setExistence(const Existence &ex) { existence = ex; }
    void setStat(const Stat &st) { stat = st; }
    void setComportment(const Comportment &comp) { comportment = comp; }

    bool alive() const { return stat.getHealth() > 0; }

    void setHomeRoom(gf::RectI room)
    {
      m_homeRoom = room;
      m_hasHome = true;
    }

    bool canWalkTo(gf::Vector2i target) const
    {
      if (!m_hasHome)
        return true;

      return (target.x > m_homeRoom.min.x &&
              target.x < m_homeRoom.max.x &&
              target.y > m_homeRoom.min.y &&
              target.y < m_homeRoom.max.y);
    }

    static Character hero(gf::Vector2i position, const gf::Texture &tex);
    static Character skeleton(gf::Vector2i position, const gf::Texture &tex);
    static Character zombie(gf::Vector2i position, const gf::Texture &tex);
    static Character slime(gf::Vector2i position, const gf::Texture &tex);

  private:
    Existence existence;
    Stat stat;
    Comportment comportment;
    const gf::Texture *texture = nullptr;
    const gf::Texture *deadTexture = nullptr;
    gf::RectI m_homeRoom;
    bool m_hasHome = false;
  };
}
#endif // CHARACTER_H
