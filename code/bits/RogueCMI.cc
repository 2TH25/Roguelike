#include "RogueCMI.h"

#include "WorldScene.h"
#include "config.h"

namespace rCMI
{
  RogueCMI::RogueCMI() 
  : gf::GameManager("RogueCMI", PATHS)
  , my_WorldScene(this)
  {
    pushScene(my_WorldScene);
  }

  // void RogueCMI::world_resource()
  // {
    
  // }
}
