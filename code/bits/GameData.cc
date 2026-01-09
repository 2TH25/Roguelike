#include "GameData.h"

namespace rCMI
{
  std::map<std::string, const char *> Controls::getControls()
  {
    std::map<std::string, const char *> res{{"mov_up", "Z"},
                                            {"mov_down", "S"},
                                            {"mov_right", "D"},
                                            {"mov_left", "Q"}};

    return res;
  }

  bool Controls::isActiveAction(std::string name, std::vector<gf::Action *> actions)
  {
    auto it = std::find_if(actions.begin(), actions.end(), [&name](const gf::Action *action)
    { 
      return action->getName() == name;
    });

    if (it == actions.end())
    {
      return false;
    }

    return (*it)->isActive();
  }
}