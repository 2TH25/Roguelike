#include "Stat.h"

namespace rCMI
{
    void Stat::addKill()
    {
        nb_kills += 1;
        addXp(50);
    }

    void Stat::addXp(int xp)
    {
        int new_xp = m_xp + xp;
        while (new_xp >= max_xp)
        {
            new_xp -= max_xp;
            max_xp *= 1.5;
            m_level += 1;
        }
        m_xp = new_xp;
    }
}