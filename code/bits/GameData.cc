#include "GameData.h"

// #include "externe/json.hpp"
// #include <fstream>

// using json = nlohmann::json;

namespace rCMI
{

  std::map<std::string, std::set<gf::Keycode>> Controls::save_actions = {};

  std::map<std::string, std::set<gf::Keycode>> Controls::getControls()
  {
    if (!save_actions.empty())
      return save_actions;

    save_actions["move_up"] = {gf::Keycode::Z, gf::Keycode::Up};
    save_actions["move_down"] = {gf::Keycode::S, gf::Keycode::Down};
    save_actions["move_left"] = {gf::Keycode::Q, gf::Keycode::Left};
    save_actions["move_right"] = {gf::Keycode::D, gf::Keycode::Right};
    save_actions["ToggleInventory"] = {gf::Keycode::Tab};
    save_actions["showMap"] = {gf::Keycode::A};
    save_actions["showMenu"] = {gf::Keycode::Escape};

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

  void Controls::resetActions(std::vector<gf::Action *> actions)
  {
    for (auto a : actions)
      if (a->isContinuous())
      {
        a->setInstantaneous();
        a->reset();
        a->setContinuous();
      }
  }
}