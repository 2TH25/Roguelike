#ifndef RL_ROGUECMI_H
#define RL_ROGUECMI_H

#include <gf/GameManager.h>

#include "WorldScene.h"
#include "BaseScene.h"
#include "MenuScene.h"

#include "GameData.h"

namespace rCMI
{
  class RogueCMI : public gf::GameManager
  {
  public:
    RogueCMI();
    BaseScene m_BaseScene;
    WorldScene m_WorldScene_L1;
    MenuScene m_MenuScene;
  };
}

#endif // RL_ROGUECMI_H
