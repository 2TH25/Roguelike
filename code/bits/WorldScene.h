#ifndef RL_WORLD_SCENE_H
#define RL_WORLD_SCENE_H

#include <gf/Scene.h>

#include "Map.h"
#include "Character.h"

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

    gf::ActionContainer getControls();

    RogueCMI *m_game = nullptr;
    Map m_map;
    Character m_hero;
    gf::ActionContainer m_actions;
  };
}

#endif // RL_WORLD_SCENE_H
