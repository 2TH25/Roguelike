#include "BaseScene.h"

#include <iostream>

namespace rCMI
{
  BaseScene::BaseScene()
  : gf::Scene({0, 0})
  , fullscreen("fullscreen")
  {
    fullscreen.addKeycodeKeyControl(gf::Keycode::F11);
    fullscreen.addKeycodeKeyControl(gf::Keycode::F);
    addAction(fullscreen);
  }

  void BaseScene::doHandleActions(gf::Window &window)
  {
    if (fullscreen.isActive())
    {
      window.setFullscreen(!window.isFullscreen());
    }
  }
}
