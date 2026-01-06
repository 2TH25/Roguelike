#include "Character.h"
#include <algorithm> // Pour std::max et std::min

namespace rCMI {

  void Character::take_damage(int damage) {
    stat.health -= damage;
    stat.health = std::max(0, std::min(stat.health, stat.max_health));

    if (stat.health <= 0) {
      die();
    }
  }

  void Character::die() {
  }

  Character Character::hero(gf::Vector2i position) {
    Character character;
    character.entity = Entity{ position, u'@', gf::Color::White, "Hero", true };
    character.stat = Stat(30, 2, 5);
    return character;
  }

  Character Character::orc(gf::Vector2i position) {
    Character character;
    character.entity = Entity{ position, u'o', gf::Color::Green, "Orc", true };
    character.stat = Stat(10, 0, 3);
    character.comportment = Comportment::hostile();
    return character;
  }

  Character Character::troll(gf::Vector2i position) {
    Character character;
    character.entity = Entity{ position, u'T', gf::Color::Blue, "Troll", true };
    character.stat = Stat(16, 1, 4);
    character.comportment = Comportment::hostile();
    return character;
  }
}