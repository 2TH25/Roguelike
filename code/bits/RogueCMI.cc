#include "RogueCMI.h"


namespace rCMI
{
  RogueCMI::RogueCMI()
      : gf::GameManager("RogueCMI", PATHS),
        m_BaseScene(),
        m_WorldScene(this),
        m_MenuScene(this),
        m_InventoryScene(std::make_unique<InventoryScene>(this)),
        m_ChestScene(this),
        m_ItemScene(this), 
        m_EndMenuScene(this),
        m_FeeScene(std::make_unique<FeeScene>(this)),
        m_VictoryScene(this),
        m_ParametersScene(this),
        m_DebutScene(this)
  {
    pushScene(m_BaseScene);
    pushScene(m_MenuScene);
  }
  RogueCMI::~RogueCMI() = default;
}
