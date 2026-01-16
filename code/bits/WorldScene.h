#ifndef RL_WORLD_SCENE_H
#define RL_WORLD_SCENE_H

#include <gf/Scene.h>

#include "Map.h"

namespace rCMI
{
  class RogueCMI;

  class WorldScene : public gf::Scene
  {
  public:
    WorldScene(RogueCMI *game);

  private:
    void doHandleActions(gf::Window &window) override;
    void doUpdate(gf::Time time) override;

    void updateFieldOfView();

    std::vector<gf::Action*> getActions();

    RogueCMI *m_game = nullptr;
    Map m_map;
    std::vector<gf::Action*> m_actions;
  };
}

#endif // RL_WORLD_SCENE_H
