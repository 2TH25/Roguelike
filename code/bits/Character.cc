#include "Character.h"
#include "WorldEntity.h"
#include "Actions.h"
#include "GameData.h"
#include <algorithm>
#include "RogueCMI.h"
#include <gf/Math.h>

namespace rCMI
{
  Character::Character(const Existence &ex, const Stat &st, const gf::Texture &tex)
      : gf::Entity(1),
        existence(ex),
        stat(st),
        texture(&tex)
  {
    m_animatedSprite.setTexture(tex);
    m_animatedSprite.setScale((float) TileSize / tex.getSize().x);
    m_animatedSprite.setPosition(existence.getPosition() * TileSize);
  }

  void Character::addAnimation(std::string name, gf::Animation animation)
  {
    m_animations[name] = animation;
  }

  void Character::playAnimation(std::string name)
  {
    if (m_animations.find(name) != m_animations.end())
    {
      m_animatedSprite.setAnimation(m_animations[name]);
    }
  }

  void Character::update(gf::Time time)
  {
    m_animatedSprite.update(time);

    if (m_isMoving)
    {
        m_moveTime += time.asSeconds();        
        float t = m_moveTime / m_moveDuration;

        if (t >= 1.0f) {
            t = 1.0f;
            m_isMoving = false;
            playAnimation("Default");
           
        }
        else {
            m_animatedSprite.setPosition(existence.getPosition() * TileSize);
        }

        gf::Vector2f currentPos = gf::lerp(m_pixelsStart, m_pixelsEnd, t);
        m_animatedSprite.setPosition(currentPos);
    }
  
  }

  bool Character::goUp(WorldEntity &m_world)
  {
    if (!alive() || m_isMoving) return false; 

    gf::Vector2i oldPos = existence.getPosition();
    m_pixelsStart = existence.getPosition() * TileSize;

    
    gf::Vector2i targetGridPos = existence.getPosition() + gf::Vector2i(0, -1);
    m_pixelsEnd = targetGridPos * TileSize;

    bool moved = bump(m_world, *this, targetGridPos);

    if (moved && existence.getPosition() != oldPos) {
        m_isMoving = true;
        m_moveTime = 0.0f;
        playAnimation("Up");
    }
    
    return moved;
  }

  bool Character::goDown(WorldEntity &m_world)
  {
    if (!alive() || m_isMoving) return false; 
    gf::Vector2i oldPos = existence.getPosition();
    m_pixelsStart = existence.getPosition() * TileSize;

    
    gf::Vector2i targetGridPos = existence.getPosition() + gf::Vector2i(0, 1);
    m_pixelsEnd = targetGridPos * TileSize;

    bool moved = bump(m_world, *this, targetGridPos);

    if (moved && existence.getPosition() != oldPos) {
        m_isMoving = true;
        m_moveTime = 0.0f;
        playAnimation("Down");
    }
    
    return moved;
  }

  bool Character::goLeft(WorldEntity &m_world)
  {
    if (!alive() || m_isMoving) return false; 
    gf::Vector2i oldPos = existence.getPosition();
    m_pixelsStart = existence.getPosition() * TileSize;

    
    gf::Vector2i targetGridPos = existence.getPosition() + gf::Vector2i(-1, 0);
    m_pixelsEnd = targetGridPos * TileSize;

    bool moved = bump(m_world, *this, targetGridPos);

    if (moved && existence.getPosition() != oldPos) {
        m_isMoving = true;
        m_moveTime = 0.0f;
        playAnimation("Left");
    }
    
    return moved;
  }

  bool Character::goRight(WorldEntity &m_world)
  {
    if (!alive() || m_isMoving) return false; 
    gf::Vector2i oldPos = existence.getPosition();
    m_pixelsStart = existence.getPosition() * TileSize;

    gf::Vector2i targetGridPos = existence.getPosition() + gf::Vector2i(1, 0);
    m_pixelsEnd = targetGridPos * TileSize;

    bool moved = bump(m_world, *this, targetGridPos);

    if (moved && existence.getPosition() != oldPos) {
        m_isMoving = true;
        m_moveTime = 0.0f;
        playAnimation("Right");
    }
    
    return moved;
  }

  void Character::doMove(WorldEntity &m_world)
  {
    if (!alive() || m_isMoving || !m_autoMove) return; 

    // 1. Sauvegarder l'ancienne position avant que l'IA ne joue
    gf::Vector2i oldPos = existence.getPosition();

    // L'IA joue (pathfinding, etc.)
    if (comportment.perform(*this, m_world))
    {
      // 2. Si l'IA a bougé le perso et que l'animation n'est pas lancée
      gf::Vector2i newPos = existence.getPosition();
      
      if (oldPos != newPos && !m_isMoving)
      {
        // On lance le mouvement visuel fluide
        startVisualMovement(oldPos, newPos);

        // On détermine la direction pour jouer la bonne animation
        gf::Vector2i diff = newPos - oldPos;
        if (diff.x > 0) playAnimation("Right");
        else if (diff.x < 0) playAnimation("Left");
        else if (diff.y > 0) playAnimation("Down");
        else if (diff.y < 0) playAnimation("Up");
      }
      return;
    }
    static gf::Random random;
    int choice = random.computeUniformInteger(0, 3);
    gf::Vector2i current = existence.getPosition();
    gf::Vector2i target = current;

    std::string animName = "Default";
    switch (choice)
    {
    case 0: target.y -= 1; animName = "Up";    break;
    case 1: target.y += 1; animName = "Down";  break;
    case 2: target.x -= 1; animName = "Left";  break;
    case 3: target.x += 1; animName = "Right"; break;
    }

    if (m_world.isWalkable(target))
    {
      m_pixelsStart = existence.getPosition() * TileSize;
      
      if (bump(m_world, *this, target))
      {
        m_pixelsEnd = existence.getPosition() * TileSize;
        m_isMoving = true;
        m_moveTime = 0.0f;
        playAnimation(animName);
      }
    }
  }

  void Character::startVisualMovement(gf::Vector2i oldGridPos, gf::Vector2i newGridPos){
    m_isMoving = true;
    m_moveTime = 0.0f;
    
    m_pixelsStart = oldGridPos * TileSize;
    m_pixelsEnd = newGridPos * TileSize;
    
    m_animatedSprite.setPosition(m_pixelsStart);
  }

  bool Character::take_damage(int damage)
  {
    int final_damage = std::max(1, damage - stat.getDefense());
    stat.setHealth(stat.getHealth() - final_damage);
    stat.setHealth(std::clamp(stat.getHealth(), 0, stat.getMaxHealth()));

    if (stat.getHealth() <= 0)
    {
      return true;
      die();
    }

    return false;
  }

  void Character::heal(int amount)
  {
    int new_health = stat.getHealth() + amount;
    stat.setHealth(std::clamp(new_health, 0, stat.getMaxHealth()));
    // game->m_InventoryScene->m_inventory.updateStatsText();
  }

  void Character::addMaxHealth(int amount)
  {
    int oldMax = stat.getMaxHealth();
    int newMax = oldMax + amount;
    if (newMax < 1)
      newMax = 1;

    stat.setMaxHealth(newMax);

    int newHealth = stat.getHealth() + amount;

    if (newHealth > newMax)
      newHealth = newMax;
    if (newHealth < 1 && alive())
      newHealth = 1;

    stat.setHealth(newHealth);
  }

  void Character::die()
  {
    existence.setCharacter(u'%');
    existence.setBlocksMovement(false);
    comportment.setVariant(std::monostate{});

    if (deadTexture != nullptr)
      texture = deadTexture;
  }
    void Character::setupMonsterAnimations(const gf::Texture &tex, int lineIndex)
  {
    float duration = 0.5f;
    float nbTilesFLine = 4.0f; 
    gf::Vector2f texSize = tex.getSize();

    gf::Vector2f tileSizeUV = { 1.0f / nbTilesFLine, 1.0f / nbTilesFLine }; 

    gf::Vector2f X = { tileSizeUV.x, 0 };
    gf::Vector2f Y = { 0, tileSizeUV.y };
    
    gf::Vector2f rowOffset = (float)lineIndex * Y;

    gf::Animation walkDown;
    walkDown.addFrame(tex, gf::RectF::fromPositionSize(rowOffset + 0.0f * X, tileSizeUV), gf::seconds(duration));
    walkDown.addFrame(tex, gf::RectF::fromPositionSize(rowOffset + 1.0f * X, tileSizeUV), gf::seconds(duration));
    addAnimation("Down", walkDown);

    gf::Animation walkLeft;
    walkLeft.addFrame(tex, gf::RectF::fromPositionSize(rowOffset + 2.0f * X, tileSizeUV), gf::seconds(duration));
    walkLeft.addFrame(tex, gf::RectF::fromPositionSize(rowOffset + 3.0f * X, tileSizeUV), gf::seconds(duration));
    addAnimation("Left", walkLeft);

    gf::Animation walkRight;
    walkRight.addFrame(tex, gf::RectF::fromPositionSize(rowOffset + 0.0f * X, tileSizeUV), gf::seconds(duration));
    walkRight.addFrame(tex, gf::RectF::fromPositionSize(rowOffset + 1.0f * X, tileSizeUV), gf::seconds(duration));
    addAnimation("Right", walkRight);

    gf::Animation walkUp;
    walkUp.addFrame(tex, gf::RectF::fromPositionSize(rowOffset + 2.0f * X, tileSizeUV), gf::seconds(duration));
    walkUp.addFrame(tex, gf::RectF::fromPositionSize(rowOffset + 3.0f * X, tileSizeUV), gf::seconds(duration));
    addAnimation("Up", walkUp);

    gf::Animation def;
    def.addFrame(tex, gf::RectF::fromPositionSize(rowOffset + 0.0f * X, tileSizeUV), gf::seconds(duration));
    addAnimation("Default", def);

    m_animatedSprite.setTextureRect(gf::RectF::fromPositionSize(rowOffset, tileSizeUV));
    
    m_animatedSprite.setScale((float)TileSize / (texSize.x / nbTilesFLine));
  }


  Character Character::hero(gf::Vector2i position, const gf::Texture &tex)
  {
    Existence ex{position, u'@', gf::Color::Blue, "Hero", true};
    Stat st(100, 0, 20);
    Character c(ex, st, tex);
    float duration = 0.5f;
    float nbTilesFLine = 4;

    gf::Vector2f texture_size = tex.getSize();
    gf::Vector2f tile_texture_size = texture_size / texture_size / nbTilesFLine;

    gf::Animation walkRight;
    gf::Vector2f X_dir = {tile_texture_size.x, 0};
    gf::Vector2f Y_dir = {0, tile_texture_size.y};  
    walkRight.addFrame(tex, gf::RectF::fromPositionSize(1 * X_dir + 1 * Y_dir, tile_texture_size), gf::seconds(duration));
    walkRight.addFrame(tex, gf::RectF::fromPositionSize(2 * X_dir + 1 * Y_dir, tile_texture_size), gf::seconds(duration));
    c.addAnimation("Right", walkRight);

    gf::Animation walkLeft;
    walkLeft.addFrame(tex, gf::RectF::fromPositionSize(3 * X_dir + 1 * Y_dir, tile_texture_size), gf::seconds(duration));
    walkLeft.addFrame(tex, gf::RectF::fromPositionSize(0 * X_dir + 2 * Y_dir, tile_texture_size), gf::seconds(duration));
    c.addAnimation("Left", walkLeft);

    gf::Animation walkUp;
    walkUp.addFrame(tex, gf::RectF::fromPositionSize(3* X_dir + 0 * Y_dir, tile_texture_size), gf::seconds(duration));
    walkUp.addFrame(tex, gf::RectF::fromPositionSize(0 * X_dir + 1 * Y_dir, tile_texture_size), gf::seconds(duration));
    c.addAnimation("Up", walkUp);

    gf::Animation walkDown;
    walkDown.addFrame(tex, gf::RectF::fromPositionSize(1 * X_dir + 0 * Y_dir, tile_texture_size), gf::seconds(duration));
    walkDown.addFrame(tex, gf::RectF::fromPositionSize(2 * X_dir + 0 * Y_dir, tile_texture_size), gf::seconds(duration));
    c.addAnimation("Down", walkDown);

    gf::Animation defaultAnimation;
    defaultAnimation.addFrame(tex, gf::RectF::fromPositionSize(0 * X_dir + 0 * Y_dir, tile_texture_size), gf::seconds(duration));
    c.addAnimation("Default", defaultAnimation);

    c.m_animatedSprite.setTextureRect(gf::RectF::fromPositionSize(0 * X_dir + 0 * Y_dir, tile_texture_size));
    c.m_animatedSprite.setScale(TileSize / (texture_size / nbTilesFLine));

    return c;
  }

  Character Character::pnj(gf::Vector2i position, const gf::Texture &tex)
  {
    Existence ex{position, u'P', gf::Color::Cyan, "PNJ", true};
    Stat st(1000, 0, 0);     
    Character c(ex, st, tex);
    c.setAutoMove(false);

    gf::Animation staticAnim;
    staticAnim.addFrame(tex, gf::RectF::fromPositionSize({0,0}, {1,1}), gf::seconds(1.0f));
    c.addAnimation("Default", staticAnim);
    c.playAnimation("Default");

    return c;
  }


  Character Character::skeleton(gf::Vector2i position, const gf::Texture &tex)
  {
    Existence ex{position, u'S', gf::Color::White, "Skeleton", true};
    Stat st(150, 2, 10);
    Character c(ex, st, tex);
    c.setComportment(Comportment::hostile());
    
    c.setupMonsterAnimations(tex, 0); 
    return c;
  }

  Character Character::zombie(gf::Vector2i position, const gf::Texture &tex)
  {
    Existence ex{position, u'Z', gf::Color::Orange, "Zombie", true};
    Stat st(100, 3, 7);
    Character c(ex, st, tex);
    c.setComportment(Comportment::hostile());

    c.setupMonsterAnimations(tex, 1); 
    return c;
  }

  Character Character::slime(gf::Vector2i position, const gf::Texture &tex)
  {
    Existence ex{position, u's', gf::Color::Green, "Slime", true};
    Stat st(50, 0, 3);
    Character c(ex, st, tex);
    c.setComportment(Comportment::hostile());

    c.setupMonsterAnimations(tex, 2); 
    return c;
  }

  void Character::render(gf::RenderTarget &target, const gf::RenderStates &states)
{
  if (!m_isMoving) {
      m_animatedSprite.setPosition(existence.getPosition() * TileSize);
  }
  m_animatedSprite.draw(target, states);
}
}