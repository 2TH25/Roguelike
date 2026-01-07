#include "Comportment.h"
#include <algorithm>
#include <gf/Vector.h>
#include <gf/VectorOps.h>

#include "Actions.h"
#include "Character.h"
#include "Map.h"

namespace rCMI {

  bool HostileEnnemy::perform(Character& self, Map& map) {
    auto target = map.hero().getExistence().getPosition(); 
    
    auto distance = 42;//gf::chebyshev_distance(self.getExistence().getPosition(), target);

    if (map.isVisible(self.getExistence().getPosition())) {
      if (distance <= 1) {
        return melee(map, self, target);
      }

      path = map.compute_path(self.getExistence().getPosition(), target);
      std::reverse(path.begin(), path.end());
    }

    if (!path.empty()) {
      target = path.back();
      path.pop_back();

      return movement(map, self, target);
    }

    return true;
  }
}