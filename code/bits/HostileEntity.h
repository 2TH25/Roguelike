#ifndef HOSTILE_ENTITY_H
#define HOSTILE_ENTITY_H

#include <vector>
#include <variant>


#include <gf/Vector.h>

namespace rCMI {
  struct Actor;
  struct Map;

  struct HostileEntity {
    std::vector<gf::Vector2i> path;

    bool perform(Actor& self, Map& map);
  };


  struct Hostile{
    std::variant<std::monostate, HostileEntity> variant;

    bool perform(Actor& self, Map& map) {
      return std::visit([&](auto&& hostile) {
        using T = std::decay_t<decltype(hostile)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
          return true;
        } else {
          return hostile.perform(self, map);
        }
      }, variant);
    }

    static HostileEntity hostile() {
      HostileEntity hostile;
      return hostile;
    }

  };


}

#endif // HOSTILE_ENTITY_H