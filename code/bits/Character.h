#ifndef CHARACTER_H
#define CHARACTER_H

#include "Comportment.h"
#include "Stat.h"
#include "Existence.h"

#include <gf/Entity.h>
#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <gf/Random.h>
#include <gf/Animation.h>
#include <gf/AnimatedSprite.h>

namespace rCMI
{
  class WorldEntity;

  class Character : public gf::Entity
  {
  public:
    Character() = default;
    Character(const Existence &ex, const Stat &st, const gf::Texture &tex);
    void update(gf::Time time) override;
    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

    void doMove(WorldEntity &m_world);
    void take_damage(int damage);
    void heal(int amount);
    void die();

    bool goUp(WorldEntity &m_world);
    bool goDown(WorldEntity &m_world);
    bool goLeft(WorldEntity &m_world);
    bool goRight(WorldEntity &m_world);
    void addAnimation(std::string name, gf::Animation animation);
    void playAnimation(std::string name);

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

    gf::AnimatedSprite m_animatedSprite;
    std::map<std::string, gf::Animation> m_animations;
    
    gf::RectI m_homeRoom;
    bool m_hasHome = false;
  };
}
#endif // CHARACTER_H
