#include <gf/Vector.h>
#include <gf/Paths.h>
#include <gf/Action.h>

#include <map>
#include <set>
#include <iostream>

namespace rCMI
{
  constexpr gf::Vector2i view_size = {800, 800};
  constexpr gf::Vector2i MapSize = {30, 30};
  constexpr gf::Vector2i TestMapSize = {10, 10};
  constexpr int TileSize = 80;

  const std::string PATH_FONT = "../data/RogueCMI/KarmaFuture.ttf";
  const std::string RESSOURCES_PATH = "../data/RogueCMI";
  const std::initializer_list<gf::Path> PATHS = {gf::Path(RESSOURCES_PATH), gf::Paths::getBasePath(), gf::Paths::getCurrentPath()};

  constexpr int RoomMaxSize = 12;
  constexpr int RoomMinSize = 4;
  constexpr int MinRooms = 5;
  constexpr int MaxRooms = 6;
  constexpr gf::Vector2i NumberRooms = {MinRooms, MaxRooms};
  constexpr int MaxMonstersMin = 2;
  constexpr int MaxMonstersPerRoom = 2;
  constexpr int leafSizeMinimum = 6;
  constexpr int leafSizeMaximum = 15;
  constexpr int NumberItems = 10;

  class Controls
  {
  public:
    static bool isActiveAction(std::string name, std::vector<gf::Action *> actions);
    // static bool setControls(std::string name, std::set<const char *> new_keys, std::vector<gf::Action *> actions);
    static std::map<std::string, std::set<const char *>> getControls();

  private:
    static std::map<std::string, std::set<const char *>> save_actions;
  };
}