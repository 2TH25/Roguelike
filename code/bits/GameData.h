#include <gf/Vector.h>
#include <gf/Paths.h>
#include <gf/Action.h>

#include <map>
#include <set>

namespace rCMI
{
  constexpr gf::Vector2i view_size = {1200, 800};
  constexpr gf::Vector2i MapSize = {100, 100};
  constexpr int TileSize = 80;

  const std::string RESSOURCES_PATH = "../data/RogueCMI";
  const std::initializer_list<gf::Path> PATHS = {gf::Path(RESSOURCES_PATH), gf::Paths::getBasePath(), gf::Paths::getCurrentPath()};

  constexpr int RoomMaxSize = 30;
  constexpr int RoomMinSize = 6;
  constexpr int MinRooms = 20;
  constexpr int MaxRooms = 30;
  constexpr gf::Vector2i NumberRooms = {MinRooms, MaxRooms};
  constexpr int MaxMonstersPerRoom = 2;
  constexpr int leafSizeMinimum = 9;
  constexpr int leafSizeMaximum = 10;

  class Controls
  {
  public:
    static bool isActiveAction(std::string name, std::vector<gf::Action *> actions);
    static std::map<std::string, const char *> getControls();
    // static bool setControls(std::map<std::string, std::set<std::string>> newControls);
  };
}