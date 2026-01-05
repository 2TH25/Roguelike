#include "Map.h"

namespace rCMI {
  struct Map {
    gf::Grid grid;
    std::vector<gf::Vector2i> compute_path(gf::Vector2i origin, gf::Vector2i target);
  };

};