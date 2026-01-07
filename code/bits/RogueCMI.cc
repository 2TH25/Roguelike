#include "RogueCMI.h"

#include "WorldScene.h"
#include "config.h"

namespace rCMI
{
  RogueCMI::RogueCMI() 
  : gf::GameManager("RogueCMI", {1600, 900})
  , my_WorldScene(this)
  {
    pushScene(my_WorldScene);
  }
}