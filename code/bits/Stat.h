#ifndef STAT_H
#define STAT_H

namespace rCMI {
  struct Character;

  struct Stat {
    Stat() = default;

    Stat(int health, int defense, int power)
    : max_health(health)
    , health(health)
    , defense(defense)
    , power(power)
    {
    }

    int max_health = 0;
    int health = 0;
    int defense = 0;
    int power = 0;
  };


}


#endif // STAT_H