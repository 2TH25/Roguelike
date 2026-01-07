#include <gf/Vector.h>
#include <gf/Paths.h>

namespace rCMI
{
  constexpr gf::Vector2i frame_size = {1200, 800};
  constexpr gf::Vector2i MapSize = {10, 10};

  const std::string RESSOURCES_PATH = "../data/RogueCMI";
  const std::initializer_list<gf::Path> PATHS = {gf::Path(RESSOURCES_PATH), gf::Paths::getBasePath(), gf::Paths::getCurrentPath()};

  constexpr int RoomMaxSize = 10;
  constexpr int RoomMinSize = 6;
  constexpr int MaxRooms = 30;
  constexpr int MaxMonstersPerRoom = 2;
}