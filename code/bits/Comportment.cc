#include "Comportment.h"
#include <algorithm>
#include <gf/Vector.h>
#include <gf/VectorOps.h> 

#include "Actions.h"
#include "Character.h"
#include "Map.h"

namespace rCMI {

  bool HostileEnnemy::perform(Character& self, Map& map) {

      auto targetPos = map.hero().getExistence().getPosition(); 
      auto selfPos = self.getExistence().getPosition();

      int distance = gf::chebyshevDistance(selfPos, targetPos);

      if (distance > 5) {
          return false; 
      }

    
      if (distance <= 1) {
        return melee(map, self, targetPos);
      }

      if (map.isVisible(selfPos)) {
        path = map.compute_path(selfPos, targetPos);
        std::reverse(path.begin(), path.end());
      }

      if (!path.empty()) {
        auto nextStep = path.back();
        path.pop_back();
        return bump(map, self, nextStep);
      }

      return true; 
  }
}