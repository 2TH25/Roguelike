#include "GameData.h"

namespace rCMI
{

  std::map<std::string, std::set<const char *>> Controls::save_actions = {};

  std::map<std::string, std::set<const char *>> Controls::getControls()
  {
    if (!save_actions.empty())
      return save_actions;

    save_actions["move_up"] = {"Z", "Up"};
    save_actions["move_down"] = {"S", "Down"};
    save_actions["move_left"] = {"Q", "Left"};
    save_actions["move_right"] = {"D", "Right"};
    save_actions["zoom_cam"] = {"="};
    save_actions["unzoom_cam"] = {"-"};
    
    return save_actions;
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