#include "GameData.h"

namespace rCMI
{
  std::map<std::string, const char *> Controls::getControls()
  {
    std::map<std::string, const char *> res{{"mov_up", "SDL_SCANCODE_Z"},
                                      {"mov_down", "SDL_SCANCODE_S"},
                                      {"mov_right", "SDL_SCANCODE_D"},
                                      {"mov_left", "SDL_SCANCODE_Q"}};

    return res;
  }
}