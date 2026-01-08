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

    gf::ActionContainer getActions();

    RogueCMI *m_game = nullptr;
    Map m_map;
    Character m_hero;
    gf::Action mov_up;
    gf::Action mov_down;
    gf::Action mov_right;
    gf::Action mov_left;
    Character m_slime;
    Character m_skeleton;
    Character m_zombie;
    gf::ActionContainer m_actions;
  };
}

#endif // RL_WORLD_SCENE_H
