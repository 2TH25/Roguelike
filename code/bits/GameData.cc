#include "GameData.h"

// #include "externe/json.hpp"
// #include <fstream>

// using json = nlohmann::json;

namespace rCMI
{

  std::map<std::string, std::set<const char *>> Controls::save_actions = {};

  std::map<std::string, std::set<const char *>> Controls::getControls()
  {
    if (!save_actions.empty())
      return save_actions;

    // std::ifstream file("../data/RogueCMI/controle.json");
    // if (!file.is_open()) {
    //   std::cerr << "Impossible d'ouvrir le fichier JSON\n";
    //   exit(1);
    // }
    // json j;
    // file >> j;

    // for (const auto& [action, touches_json] : j.items())
    // {
    //   save_actions[action] = {};
    //   for (const auto& t : touches_json)
    //   {
    //     std::string t_string = t.get<std::string>();
    //     char* t_copy = new char[t_string.size()];
    //     std::strcpy(t_copy, t_string.c_str());

    //     save_actions[action].insert(t_copy);
    //   }
    // }

    save_actions["move_up"] = {"Z", "Up"};
    save_actions["move_down"] = {"S", "Down"};
    save_actions["move_left"] = {"Q", "Left"};
    save_actions["move_right"] = {"D", "Right"};
    save_actions["zoom_cam"] = {"="};
    save_actions["unzoom_cam"] = {"-"};
    
    return save_actions;
  }

  // bool Controls::setControls(std::string name, std::set<const char *> new_keys, std::vector<gf::Action *> actions)
  // {
  //   if (save_actions.empty() || save_actions.count(name) == 0)
  //     return false;

  //   auto it = std::find_if(actions.begin(), actions.end(), [&name](const gf::Action *action)
  //                          { return action->getName() == name; });

  //   if (it == actions.end())
  //     return false;

  //   save_actions.at(name) = new_keys;


  // }

  bool Controls::isActiveAction(std::string name, std::vector<gf::Action *> actions)
  {
    auto it = std::find_if(actions.begin(), actions.end(), [&name](const gf::Action *action)
                           { return action->getName() == name; });

    if (it == actions.end())
      return false;

    return (*it)->isActive();
  }
}