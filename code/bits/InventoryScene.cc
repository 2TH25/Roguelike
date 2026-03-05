#include "InventoryScene.h"
#include <gf/Coordinates.h>
#include "RogueCMI.h"

namespace rCMI
{
  InventoryScene::InventoryScene(RogueCMI *game)
      : gf::Scene(view_size),
        m_game(game),
        font(m_game->resources.getFont(PATH_FONT)),
        m_inventory(game),
        esc("escape"),
        escActive(false),
        map("map"),
        mapActive(false)
  {
    setClearColor(gf::Color::Black);
    esc.addKeycodeKeyControl(gf::Keycode::Tab);
    esc.addKeycodeKeyControl(gf::Keycode::Escape);
    addAction(esc);

    map.addKeycodeKeyControl(gf::Keycode::A);
    addAction(map);
  }

  void InventoryScene::doHandleActions([[maybe_unused]] gf::Window &window)
  {
    escActive = esc.isActive();
    mapActive = map.isActive();
  }

  void InventoryScene::doUpdate([[maybe_unused]] gf::Time time)
  {
    if (escActive && isActive()) m_game->popScene();
    if (mapActive && isActive())
    {
      m_game->popScene();
      m_game->m_WorldScene.resume();
      m_game->m_WorldScene.m_isActivateMap = 1;
      m_game->m_WorldScene.update(time);
    }
  }

  void InventoryScene::doProcessEvent(gf::Event &event)
  {
    if (event.type == gf::EventType::MouseButtonPressed && isActive())
    {
      gf::Vector2f coords = m_game->computeWindowToGameCoordinates(event.mouseButton.coords, getHudView());
      m_inventory.handleItemClick(coords, m_game, this);
    }
  }

  void InventoryScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    target.setView(getHudView());
    m_inventory.render(target, states);
  }
}