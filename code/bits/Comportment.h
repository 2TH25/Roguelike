#ifndef COMPORTMENT_H
#define COMPORTMENT_H

#include <vector>
#include <variant>
#include <gf/Vector.h>

namespace rCMI {
  class Character;
  class Map;

  class HostileEnnemy {
  public:
    const std::vector<gf::Vector2i> getPath() const { return path; }
  
    bool perform(Character& self, Map& map);

  private:
    std::vector<gf::Vector2i> path;
  };

  class Comportment {

  public:

    const std::variant<std::monostate, HostileEnnemy>& getVariant() const { return variant; }
    void setVariant(const std::variant<std::monostate, HostileEnnemy>& var) { variant = var; }

    bool perform(Character& self, Map& map) {
      return std::visit([&](auto&& comportement) {
        using T = std::decay_t<decltype(comportement)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
          return true;
        } else {
          return comportement.perform(self, map);
        }
      }, variant);
    }

    static Comportment hostile() {
      Comportment comportment;
      comportment.variant = HostileEnnemy();
      return comportment;
    }

    private:
        std::variant<std::monostate, HostileEnnemy> variant;
  };
}

#endif // COMPORTMENT_H