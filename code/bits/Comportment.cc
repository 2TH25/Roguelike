#include "Comportment.h"
#include <algorithm>
#include <gf/Vector.h>
#include <gf/VectorOps.h>

#include "Actions.h"
#include "Character.h"
#include "WorldEntity.h"

namespace rCMI
{

  bool HostileEnnemy::perform(Character &self, WorldEntity &m_world)
  {
    auto targetPos = m_world.hero().getExistence().getPosition();
    auto selfPos = self.getExistence().getPosition();

    int distance = gf::chebyshevDistance(selfPos, targetPos);

    if (distance > 5)
      return false;

    if (distance <= 1)
      return melee(m_world, self, targetPos);

    if (m_world.isVisible(selfPos))
    {
      path = m_world.compute_path(selfPos, targetPos);
      std::reverse(path.begin(), path.end());
    }

    if (!path.empty())
    {
      auto nextStep = path.back();
      path.pop_back();
      return bump(m_world, self, nextStep);
    }

    return true;
  }
}