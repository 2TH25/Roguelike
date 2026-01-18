#ifndef RL_ROGUECMI_H
#define RL_ROGUECMI_H

#include <gf/GameManager.h>

#include "WorldScene.h"
#include "BaseScene.h"
#include "MenuScene.h"
#include "TestZone.h"
#include "InventoryScene.h"

#include "GameData.h"

namespace rCMI
{
  class RogueCMI : public gf::GameManager
  {
  public:
    RogueCMI();
    
    BaseScene m_BaseScene;
    WorldScene m_WorldScene;
    MenuScene m_MenuScene;
    TestScene m_TestScene;
    InventoryScene m_InventoryScene;
  };
}

#endif // RL_ROGUECMI_H
