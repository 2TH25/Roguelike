#include "Character.h"
#include "WorldEntity.h"
#include "Actions.h"
#include "GameData.h"
#include <algorithm>

namespace rCMI
{
  Character::Character(const Existence &ex, const Stat &st, const gf::Texture &tex)
      : gf::Entity(1),
        existence(ex),
        stat(st),
        texture(&tex)
  {
    m_animatedSprite.setTexture(tex);
    m_animatedSprite.setPosition(existence.getPosition() * TileSize);
  }

  void Character::addAnimation(std::string name, gf::Animation animation) {
      m_animations[name] = animation;
  }

  void Character::playAnimation(std::string name) {
      if (m_animations.find(name) != m_animations.end()) {
          m_animatedSprite.setAnimation(m_animations[name]);
      }
  }

  void Character::update(gf::Time time) {
      m_animatedSprite.update(time);
  }


  bool Character::goUp(WorldEntity &m_world)
  {
    if (!alive())
      return false;

    gf::Vector2i current = existence.getPosition();
    gf::Vector2i target = {current.x, current.y - 1};
    return bump(m_world, *this, target);
  }

  bool Character::goDown(WorldEntity &m_world)
  {
    if (!alive())
      return false;

    gf::Vector2i current = existence.getPosition();
    gf::Vector2i target = {current.x, current.y + 1};
    return bump(m_world, *this, target);
  }

  bool Character::goLeft(WorldEntity &m_world)
  {
    if (!alive())
      return false;

    gf::Vector2i current = existence.getPosition();
    gf::Vector2i target = {current.x - 1, current.y};
    return bump(m_world, *this, target);
  }

  bool Character::goRight(WorldEntity &m_world)
  {
    if (!alive())
      return false;

    gf::Vector2i current = existence.getPosition();
    gf::Vector2i target = {current.x + 1, current.y};
    return bump(m_world, *this, target);
  }

  void Character::doMove(WorldEntity &m_world)
  {
    if (!alive())
      return;

    if (comportment.perform(*this, m_world))
      return;

    static gf::Random random;
    int choice = random.computeUniformInteger(0, 3);

    gf::Vector2i current = existence.getPosition();
    gf::Vector2i target = current;

    switch (choice)
    {
    case 0:
      target.y -= 1;
      break;
    case 1:
      target.y += 1;
      break;
    case 2:
      target.x -= 1;
      break;
    case 3:
      target.x += 1;
      break;
    }
    if (m_world.isWalkable(target))
      bump(m_world, *this, target);
  }

  void Character::take_damage(int damage)
  {
    int final_damage = std::max(0, damage - stat.getDefense());
    stat.setHealth(stat.getHealth() - final_damage);
    stat.setHealth(std::clamp(stat.getHealth(), 0, stat.getMaxHealth()));

    if (stat.getHealth() <= 0)
      die();
  }

  void Character::heal(int amount)
  {
    int new_health = stat.getHealth() + amount;
    stat.setHealth(std::clamp(new_health, 0, stat.getMaxHealth()));
  }

  void Character::die()
  {
    existence.setCharacter(u'%');
    existence.setBlocksMovement(false);
    comportment.setVariant(std::monostate{});

    if (deadTexture != nullptr)
      texture = deadTexture;
  }

  Character Character::hero(gf::Vector2i position, const gf::Texture &tex)
  {
    Existence ex{position, u'@', gf::Color::Blue, "Hero", true};
    Stat st(100, 0, 20);
    Character c(ex, st, tex);

    gf::Vector2f frameSize = {80.0f, 80.0f};    
    float duration = 0.2f;

    gf::Animation walkRight;
    walkRight.addFrame(tex, gf::RectF::fromPositionSize({80.0f, 80.0f}, frameSize), gf::seconds(duration)); 
    walkRight.addFrame(tex, gf::RectF::fromPositionSize({160.0f, 80.0f}, frameSize), gf::seconds(duration));
    c.addAnimation("Right", walkRight);

    gf::Animation walkLeft;
    walkLeft.addFrame(tex, gf::RectF::fromPositionSize({240.0f, 80.0f}, frameSize), gf::seconds(duration)); 
    walkLeft.addFrame(tex, gf::RectF::fromPositionSize({0.0f, 160.0f}, frameSize), gf::seconds(duration));
    c.addAnimation("Left", walkLeft);

    gf::Animation walkUp;
    walkUp.addFrame(tex, gf::RectF::fromPositionSize({240.0f, 0.0f}, frameSize), gf::seconds(duration)); 
    walkUp.addFrame(tex, gf::RectF::fromPositionSize({0.0f, 80.0f}, frameSize), gf::seconds(duration));
    c.addAnimation("Up", walkUp);

    gf::Animation walkDown;
    walkDown.addFrame(tex, gf::RectF::fromPositionSize({80.0f, 0.0f}, frameSize), gf::seconds(duration));
    walkDown.addFrame(tex, gf::RectF::fromPositionSize({160.0f, 0.0f}, frameSize), gf::seconds(duration));
    c.addAnimation("Down", walkDown);

    gf::Animation defaultAnimation;
    defaultAnimation.addFrame(tex, gf::RectF::fromPositionSize({0.0f, 0.0f}, frameSize), gf::seconds(duration));
    c.addAnimation("Default", defaultAnimation);

    c.m_animatedSprite.setTextureRect(gf::RectF::fromPositionSize({0.0f, 0.0f}, frameSize));


    return c;
  }
  

  Character Character::skeleton(gf::Vector2i position, const gf::Texture &tex)
  {
    Existence ex{position, u'S', gf::Color::White, "Skeleton", true};
    Stat st(150, 2, 10);
    Comportment comp(Comportment::hostile());
    Character c(ex, st, tex);
    c.setComportment(comp);
    return c;
  }

  Character Character::zombie(gf::Vector2i position, const gf::Texture &tex)
  {
    Existence ex{position, u'Z', gf::Color::Orange, "Zombie", true};
    Stat st(100, 3, 7);
    Comportment comp(Comportment::hostile());

    Character c(ex, st, tex);
    c.setComportment(comp);
    return c;
  }

  Character Character::slime(gf::Vector2i position, const gf::Texture &tex)
  {
    Existence ex{position, u's', gf::Color::Green, "Slime", true};
    Stat st(50, 0, 3);
    Comportment comp(Comportment::hostile());

    Character c(ex, st, tex);
    c.setComportment(comp);
    return c;
  }
  void Character::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    gf::Vector2i gridPosition = existence.getPosition();
    m_animatedSprite.setPosition({
        static_cast<float>(gridPosition.x * TileSize), 
        static_cast<float>(gridPosition.y * TileSize)
    });

    float scale = static_cast<float>(TileSize) / 640.0f;
    m_animatedSprite.setScale(scale);

    target.draw(m_animatedSprite, states);
  } 
}
