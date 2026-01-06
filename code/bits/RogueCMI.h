#ifndef RL_ROGUECMI_H
#define RL_ROGUECMI_H

#include <gf/SceneManager.h>

#include "WorldScene.h"

namespace rCMI
{
  class RogueCMI : public gf::SceneManager
  {
  public:
    RogueCMI();

  private:
    std::unique_ptr<WorldScene> my_WorldScene = nullptr;
  };
}

#endif // RL_ROGUECMI_H
