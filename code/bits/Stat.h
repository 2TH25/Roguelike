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
    int getXp() const { return m_xp; }
    int getMaxXp() const { return max_xp; }
    int getLevel() const { return m_level; }
    int getDefense() const { return defense; }
    int getPower() const { return power; }
    int getKills() const { return nb_kills; }
    int getSkillPoints() const {return m_skillpoints;}
  

    void setHealth(int h) { health = h; }
    void setMaxHealth(int mh) { max_health = mh; }
    void setDefense(int d) { defense = d; }
    void setPower(int p) { power = p; }
    void addStat(int h, int d, int p) { health += h; max_health += h; defense += d; power += p; }
    void retireStat(int h, int d, int p) { health -= h; max_health -= h; defense -= d; power -= p; }
    void addKill();
    void addXp(int xp);
    void setSkillPoints(int s) { m_skillpoints = s;}

  private:
    int max_health = 0;
    int health = 0;
    int defense = 0;
    int power = 0;

    int nb_kills = 0;
    int m_level = 0;
    int m_xp = 0;
    int max_xp = 200;
    int m_skillpoints = 0;
  };

}

#endif // STAT_H