#ifndef RL_WORLD_SCENE_H
#define RL_WORLD_SCENE_H

#include <gf/Scene.h>
#include <gf/Entity.h>
#include <gf/RenderTarget.h>
#include <gf/RenderStates.h>

namespace rCMI
{
  class RogueCMI;

  class Player : public gf::Entity
  {
  public:
    Player();
    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;
  };

  class WorldScene : public gf::Scene
  {
  public:
    WorldScene(RogueCMI *game);

  private:
    void doHandleActions(gf::Window &window) override;
    void doUpdate(gf::Time time) override;

    void updateFieldOfView();

    RogueCMI *m_game = nullptr;
    Player m_player;
  };
}

#endif // RL_WORLD_SCENE_H
