#ifndef RL_WORLD_SCENE_H
#define RL_WORLD_SCENE_H

#include <gf/Scene.h>

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

    RogueCMI *m_game = nullptr;
  };
}

#endif // RL_WORLD_SCENE_H
