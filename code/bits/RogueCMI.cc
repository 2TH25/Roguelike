#include "RogueCMI.h"

#include "WorldScene.h"

namespace rCMI
{
  RogueCMI::RogueCMI()
      : gf::GameManager("RogueCMI", PATHS),
        m_BaseScene(),
        m_WorldScene_L1(this),
        m_MenuScene(this),
        m_TestScene(this)
  {
    pushScene(m_BaseScene);
    pushScene(m_MenuScene);
  }
}
