#include "Comportment.h"

#include <algorithm>

#include <gf/Vector.h>

#include "Actions.h"
#include "Character.h"
#include "Map.h"

namespace rCMI {

  bool HostileEnnemy::perform(Character& self, Map& map)
  {
    auto target = map.hero().entity.position;
    auto distance = gf::chebyshev_distance(self.entity.position, target);

    if (map.grid.visible(self.entity.position)) {
      if (distance <= 1) {
        return melee(map, self, target);
      }

      path = map.compute_path(self.entity.position, target);
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