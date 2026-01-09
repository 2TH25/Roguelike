#include "GameData.h"

namespace rCMI
{
  std::map<std::string, std::set<const char *>> Controls::getControls()
  {
    std::map<std::string, std::set<const char *>> res{{"move_up", {"Z", "Up"}},
                                                      {"move_down", {"S", "Down"}},
                                                      {"move_right", {"D", "Right"}},
                                                      {"move_left", {"Q", "Left"}}};

    return res;
  }

  bool Controls::isActiveAction(std::string name, std::vector<gf::Action *> actions)
  {
    auto it = std::find_if(actions.begin(), actions.end(), [&name](const gf::Action *action)
                           { return action->getName() == name; });

    if (it == actions.end())
      return false;

    return (*it)->isActive();
  }
}