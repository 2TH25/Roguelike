#ifndef STAT_H
#define STAT_H

namespace rCMI
{
  class Stat
  {
  public:
    Stat() = default;

    Stat(int health, int defense, int power)
        : max_health(health),
          health(health),
          defense(defense),
          power(power)
    {
    }

    int getHealth() const { return health; }
    int getMaxHealth() const { return max_health; }
    int getDefense() const { return defense; }
    int getPower() const { return power; }

    void setHealth(int h) { health = h; }

  private:
    int max_health = 0;
    int health = 0;
    int defense = 0;
    int power = 0;
  };

}

#endif // STAT_H