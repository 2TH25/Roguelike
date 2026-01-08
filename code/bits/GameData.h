#include <gf/Vector.h>
#include <gf/Paths.h>

#include <map>
#include <set>

namespace rCMI
{
  constexpr gf::Vector2i view_size = {1200, 800};
  constexpr gf::Vector2i MapSize = {10, 10};

  const std::string RESSOURCES_PATH = "../data/RogueCMI";
  const std::initializer_list<gf::Path> PATHS = {gf::Path(RESSOURCES_PATH), gf::Paths::getBasePath(), gf::Paths::getCurrentPath()};

  constexpr int RoomMaxSize = 10;
  constexpr int RoomMinSize = 6;
  constexpr int MaxRooms = 30;
  constexpr int MaxMonstersPerRoom = 2;

  class Controls {
    public:
      static std::map<std::string, std::string> getControls();
      // static bool setControls(std::map<std::string, std::set<std::string>> newControls);
  };
}