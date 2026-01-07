#include "Character.h"

#include <algorithm>

namespace rCMI {

  void Character::take_damage(int damage) {
    int final_damage = std::max(0, damage - stat.getDefense());
    stat.setHealth(stat.getHealth() - final_damage);
    stat.setHealth(std::clamp(stat.getHealth(), 0, stat.getMaxHealth()));

    if (stat.getHealth() <= 0) {
      die();
    }
  }

  void Character::die() {
    entity.setCharacter(u'%'); // Utilise le setter
    entity.setBlocksMovement(false);
    comportment.setVariant(std::monostate{});
  }
  Character Character::hero(gf::Vector2i position) {
    Character character;
    character.entity = Entity{ position, u'@', gf::Color::Blue, "Hero", true };
    character.stat = Stat(100, 5, 10); // HP: 100, Def: 5, Pwr: 10
    return character;
  }

  // Skeleton (Beaucoup de vie)
  Character Character::skeleton(gf::Vector2i position) {
    Character character;
    character.entity = Entity{ position, u'S', gf::Color::White, "Skeleton", true };
    character.stat = Stat(50, 2, 6); 
    character.comportment = Comportment::hostile();
    return character;
  }

  // Zombie (Vie moyenne)
  Character Character::zombie(gf::Vector2i position) {
    Character character;
    character.entity = Entity{ position, u'Z', gf::Color::Orange, "Zombie", true };
    character.stat = Stat(30, 1, 4);
    character.comportment = Comportment::hostile();
    return character;
  }

  // Slime (Peu de vie)
  Character Character::slime(gf::Vector2i position) {
    Character character;
    character.entity = Entity{ position, u's', gf::Color::Green, "Slime", true };
    character.stat = Stat(10, 0, 2);
    character.comportment = Comportment::hostile();
    return character;
  }
}