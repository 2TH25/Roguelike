#include "RogueCMI.h"

#include "WorldScene.h"
#include "config.h"

namespace rCMI
{
  RogueCMI::RogueCMI() : gf::SceneManager("RogueCMI", {1600, 900})
  {
    my_WorldScene = std::make_unique<WorldScene>(this);
    // pushScene(my_WorldScene.get());
  }
}