#ifndef RL_FULLSCREEN_SCENE_H
#define RL_FULLSCREEN_SCENE_H

#include <gf/Scene.h>


namespace rCMI
{
  class BaseScene : public gf::Scene
  {
  public:
    BaseScene();

  private:
    void doHandleActions(gf::Window &window) override;

    gf::Action fullscreen;
  };
}

#endif // RL_FULLSCREEN_SCENE_H
