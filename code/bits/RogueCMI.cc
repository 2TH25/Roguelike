#include "RogueCMI.h"


namespace rCMI
{
  RogueCMI::RogueCMI()
      : gf::GameManager("RogueCMI", PATHS),
        m_BaseScene(),
        m_WorldScene(this),
        m_MenuScene(this),
        m_InventoryScene(std::make_unique<InventoryScene>(this)),
        m_ChestScene(this)
  {
    pushScene(m_BaseScene);
    pushScene(m_MenuScene);
  }
  RogueCMI::~RogueCMI() = default;
}
