#include "Character.h"
#include "Map.h"
#include "Actions.h"
#include <algorithm>

namespace rCMI {
  Character::Character(const Existence& ex, const Stat& st, const gf::Texture& tex) 
  : gf::Entity(1)
  , existence(ex)
  , stat(st)      
  , texture(&tex)
  {
  }

  void Character::goUp(Map& map) { 
      gf::Vector2i current = existence.getPosition();
      gf::Vector2i target = { current.x, current.y - 1 };
      bump(map, *this, target); 
  }

  void Character::goDown(Map& map) {
      gf::Vector2i current = existence.getPosition();
      gf::Vector2i target = { current.x, current.y + 1 };
      
      bump(map, *this, target);
  }

  void Character::goLeft(Map& map) { 
      gf::Vector2i current = existence.getPosition();
      gf::Vector2i target = { current.x - 1, current.y };
      
      bump(map, *this, target);
  }

  void Character::goRight(Map& map) { 
      gf::Vector2i current = existence.getPosition();
      gf::Vector2i target = { current.x + 1, current.y };
      
      bump(map, *this, target);
  }

  void Character::take_damage(int damage) {
    int final_damage = std::max(0, damage - stat.getDefense());
    stat.setHealth(stat.getHealth() - final_damage);
    stat.setHealth(std::clamp(stat.getHealth(), 0, stat.getMaxHealth()));

    if (stat.getHealth() <= 0) {
      die();
    }
  }

  void Character::die() {
    existence.setCharacter(u'%'); 
    existence.setBlocksMovement(false);
    comportment.setVariant(std::monostate{});
  }


  Character Character::hero(gf::Vector2i position, const gf::Texture& tex) {
    Existence ex{ position, u'@', gf::Color::Blue, "Hero", true };
    Stat st(100, 5, 10);
    return Character(ex, st, tex);
  }

  Character Character::skeleton(gf::Vector2i position, const gf::Texture& tex) {
    Existence ex{ position, u'S', gf::Color::White, "Skeleton", true };
    Stat st(50, 2, 6);
    Comportment comp(Comportment::hostile());    
    Character c(ex, st, tex);
    c.setComportment(comp);
    return c;
  }

  Character Character::zombie(gf::Vector2i position, const gf::Texture& tex) {
      Existence ex{ position, u'Z', gf::Color::Orange, "Zombie", true };
      Stat st(30, 1, 4);
      Comportment comp(Comportment::hostile());
      
      Character c(ex, st, tex);
      c.setComportment(comp); 
      return c;
  }

  Character Character::slime(gf::Vector2i position, const gf::Texture& tex) {
      Existence ex{ position, u's', gf::Color::Green, "Slime", true };
      Stat st(10, 0, 2);
      Comportment comp(Comportment::hostile());

      Character c(ex, st, tex);
      c.setComportment(comp); 
      return c;
  }
  void Character::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    if (!texture) {
        return;
    }

    gf::Sprite sprite(*texture);
    gf::Vector2i gridPosition = existence.getPosition();
    float tileSize = 80;
    sprite.setPosition({ gridPosition.x * tileSize, gridPosition.y * tileSize });
    target.draw(sprite, states);
}
}
