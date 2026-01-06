#include "Character.h"

namespace rCMI {

  void Character::take_damage(int damage)
  {
    stat.health -= damage;
    stat.health = std::max(0, std::min(stat.health, stat.max_health));

    // TODO

  }

  void Character::die()
  {
  }

  Character Character::hero(gf::Vector2i position)
  {
    Character character;

    //Position non correcte donc non valide ici
    character.entity = { position, '@', gf::Color::White, "Hero", true };
    character.stat = Stat(30, 2, 5);
    return character;
  }

  Character Character::orc(gf::Vector2i position)
  {
    Character character;
    character.entity = { position, 'o', gf::Color::Green, "Orc", true };
    character.stat = Stat(10, 0, 3);
    character.comportment = Comportment::hostile();
    return character;
  }

  Character Character::troll(gf::Vector2i position)
  {
    Character character;
    character.entity = { position, 'T', gf::Color::Blue, "Troll", true };
    character.stat = Stat(16, 1, 4);
    character.comportment = Comportment::hostile();
    return character;
  }

}