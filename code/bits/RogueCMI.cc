#include "RogueCMI.h"

#include "WorldScene.h"

namespace rCMI
{
  RogueCMI::RogueCMI() 
  : gf::GameManager("RogueCMI", PATHS)
  , m_BaseScene()
  , m_WorldScene(this)
  {
    pushScene(m_BaseScene);
    pushScene(m_WorldScene);
  }
}
