#include "BaseScene.h"

#include <iostream>

namespace rCMI
{
  BaseScene::BaseScene()
  : gf::Scene({0, 0})
  , fullscreen("fullscreen")
  {
    fullscreen.addScancodeKeyControl(gf::Scancode::F11);
    fullscreen.addScancodeKeyControl(gf::Scancode::F);
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
