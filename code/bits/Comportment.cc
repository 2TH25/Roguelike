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

      if (m_world.isVisible(selfPos))
      {
          self.setAggressive(true);
      }

      if (!self.isAggressive())
      {
          return false;
      }


      int distance = gf::chebyshevDistance(selfPos, targetPos);
      if (distance > 5)
        return false;

      if (distance <= 1)
        return melee(m_world, self, targetPos); 

      path = m_world.compute_path(selfPos, targetPos);
      
      if (!path.empty())
      {
        if (path.size() > 0) {
            auto nextStep = path[0];
            return bump(m_world, self, nextStep);
        }
      }

      return true;
  }

}