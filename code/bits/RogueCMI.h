#ifndef RL_ROGUECMI_H
#define RL_ROGUECMI_H

#include <gf/GameManager.h>
#include <gf/ResourceManager.h>

#include "WorldScene.h"
#include "GameData.h"

namespace rCMI
{
  class RogueCMI : public gf::GameManager
  {
  public:
    RogueCMI();

  private:
    gf::ResourceManager world_resource();

    WorldScene my_WorldScene;
  };
}

#endif // RL_ROGUECMI_H
