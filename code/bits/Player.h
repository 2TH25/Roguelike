#ifndef PLAYER_H
#define PLAYER_H

namespace rCMI {
  struct Actor;

  struct Player {
    Player() = default;

    Player(int health, int defense, int power, int vision_radius)
    : max_health(health)
    , health(health)
    , defense(defense)
    , power(power)
    , vision_radius(vision_radius)
    {
    }

    int max_health = 0;
    int health = 0;
    int defense = 0;
    int power = 0;
    int vision_radius = 1;
  };


}


#endif // PLAYER_H