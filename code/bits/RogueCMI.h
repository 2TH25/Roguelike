#ifndef RL_ROGUECMI_H
#define RL_ROGUECMI_H

#include <gf/GameManager.h>

#include "WorldScene.h"
#include "BaseScene.h"
#include "MenuScene.h"
#include "InventoryScene.h"
#include "ChestScene.h"
#include "GameData.h"
#include "ItemScene.h"
#include "EndMenuScene.h"
#include "feeScene.h"

namespace rCMI
{
  class InventoryScene;
  class FeeScene;

  class RogueCMI : public gf::GameManager
  {
  public:
    RogueCMI();
    ~RogueCMI();
    
    BaseScene m_BaseScene;
    WorldScene m_WorldScene;
    MenuScene m_MenuScene;
    std::unique_ptr<InventoryScene> m_InventoryScene;
    ChestScene m_ChestScene;
    ItemScene m_ItemScene;
    EndMenuScene m_EndMenuScene;
    std::unique_ptr<FeeScene> m_FeeScene;
  };
}

#endif // RL_ROGUECMI_H
