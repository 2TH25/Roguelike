#include "InventoryScene.h"
#include <gf/Coordinates.h>
#include "RogueCMI.h"

namespace rCMI
{
  InventoryScene::InventoryScene(RogueCMI *game)
      : gf::Scene(view_size),
        m_game(game),
        font(PATH_FONT),
        m_inventory(game)
  {
    setClearColor(gf::Color::Black);
  }

  void InventoryScene::doProcessEvent(gf::Event &event)
  {
    if (event.type == gf::EventType::MouseMoved || event.type == gf::EventType::MouseButtonPressed)
      gf::Vector2f coords = m_game->computeWindowToGameCoordinates(event.mouseCursor.coords, getHudView());
  }

  void InventoryScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    target.setView(getHudView());
    m_inventory.render(target, states);
  }
}