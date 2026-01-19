#include "Character.h"
#include "Map.h"
#include "Actions.h"
#include "GameData.h"
#include <algorithm>

namespace rCMI {
  Character::Character(const Existence& ex, const Stat& st, const gf::Texture& tex) 
  : gf::Entity(1)
  , existence(ex)
  , stat(st)      
  , texture(&tex)
  {
  }

  bool Character::goUp(Map& map) { 
    if (!alive()) return false;
    gf::Vector2i current = existence.getPosition();
    gf::Vector2i target = { current.x, current.y - 1 };
    return bump(map, *this, target); 
  }

  bool Character::goDown(Map& map) {
    if (!alive()) return false;
    gf::Vector2i current = existence.getPosition();
    gf::Vector2i target = { current.x, current.y + 1 };
    return bump(map, *this, target);
  }

  bool Character::goLeft(Map& map) { 
    if (!alive()) return false;
    gf::Vector2i current = existence.getPosition();
    gf::Vector2i target = { current.x - 1, current.y };
    return bump(map, *this, target);
  }

  bool Character::goRight(Map& map) { 
    if (!alive()) return false;
    gf::Vector2i current = existence.getPosition();
    gf::Vector2i target = { current.x + 1, current.y };
    return bump(map, *this, target);
  }

void Character::doMove(Map& map) {
    if (!alive()) return;

    if (comportment.perform(*this, map)) {
        return;
    }
    static gf::Random random; 
    int choice = random.computeUniformInteger(0, 3);
    
    gf::Vector2i current = existence.getPosition();
    gf::Vector2i target = current;

    switch (choice) {
        case 0: target.y -= 1; break;
        case 1: target.y += 1; break;
        case 2: target.x -= 1; break;
        case 3: target.x += 1; break;
    }
    if (map.isWalkable(target)) {
        bump(map, *this, target);
    }
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

    if (deadTexture != nullptr) {
        texture = deadTexture;
    }
  }

  Character Character::hero(gf::Vector2i position, const gf::Texture& tex) {
    Existence ex{ position, u'@', gf::Color::Blue, "Hero", true };
    Stat st(100, 0, 20);
    return Character(ex, st, tex);
  }

  Character Character::skeleton(gf::Vector2i position, const gf::Texture& tex) {
    Existence ex{ position, u'S', gf::Color::White, "Skeleton", true };
    Stat st(150, 2, 10);
    Comportment comp(Comportment::hostile());    
    Character c(ex, st, tex);
    c.setComportment(comp);
    return c;
  }

  Character Character::zombie(gf::Vector2i position, const gf::Texture& tex) {
      Existence ex{ position, u'Z', gf::Color::Orange, "Zombie", true };
      Stat st(100, 3, 7);
      Comportment comp(Comportment::hostile());
      
      Character c(ex, st, tex);
      c.setComportment(comp); 
      return c;
  }

  Character Character::slime(gf::Vector2i position, const gf::Texture& tex) {
      Existence ex{ position, u's', gf::Color::Green, "Slime", true };
      Stat st(50, 0, 3);
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
    float tileSize = TileSize;
    sprite.setPosition({ gridPosition.x * tileSize, gridPosition.y * tileSize });
    target.draw(sprite, states);
}
}
