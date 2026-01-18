#ifndef RL_WORLD_SCENE_H
#define RL_WORLD_SCENE_H

#include <gf/Scene.h>
#include <gf/Event.h>
#include "Map.h"

namespace rCMI
{
  class RogueCMI;

  class WorldScene : public gf::Scene
  {
  public:
    WorldScene(RogueCMI *game);
    void generateMap(gf::Vector2i size);

  private:
    void doHandleActions(gf::Window &window) override;
    void doUpdate(gf::Time time) override;

    void updateFieldOfView();
    void doProcessEvent(gf::Event& event) override;

    std::vector<gf::Action*> getActions();

    RogueCMI *m_game = nullptr;
    Map m_map;
    std::vector<gf::Action*> m_actions;
    gf::Vector2i m_mousePosition;
    gf::Action* m_fireAction = nullptr;
    gf::Action toggleInventory;

    gf::Time m_timeSinceDeath;
    bool m_gameOverHandled;
  };
}

#endif // RL_WORLD_SCENE_H
