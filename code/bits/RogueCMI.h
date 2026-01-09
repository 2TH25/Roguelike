#ifndef RL_ROGUECMI_H
#define RL_ROGUECMI_H

#include <gf/GameManager.h>

#include "WorldScene.h"
#include "BaseScene.h"

#include "GameData.h"

namespace rCMI
{
  class RogueCMI : public gf::GameManager
  {
  public:
    RogueCMI();
    gf::Random random;
  private:
    BaseScene m_BaseScene;
    WorldScene m_WorldScene;
  };
}

#endif // RL_ROGUECMI_H
