#include "Comportment.h"
#include <algorithm>
#include <gf/Vector.h>
#include <gf/VectorOps.h>

#include "Actions.h"
#include "Character.h"
#include "Map.h"

namespace rCMI {

  bool HostileEnnemy::perform(Character& self, Map& map) {
    auto target = map.hero().getEntity().getPosition(); 
    
    auto distance = 6;//gf::chebyshev_distance(self.getEntity().getPosition(), target);

    if (map.isVisible(self.getEntity().getPosition())) {
      if (distance <= 1) {
        return melee(map, self, target);
      }

      path = map.compute_path(self.getEntity().getPosition(), target);
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