#include "InventoryScene.h"
#include <gf/Coordinates.h>
#include "RogueCMI.h"

namespace rCMI
{
  InventoryScene::InventoryScene(RogueCMI *game)
      : gf::Scene(view_size),
        m_game(game), 
        toggleInventory("ToggleInventory"),
        font(PATH_FONT),
        m_inventory(game)
  {
    setClearColor(gf::Color::Black);

    toggleInventory.addKeycodeKeyControl(gf::Keycode::Tab);
    addAction(toggleInventory);
  }

  void InventoryScene::doHandleActions([[maybe_unused]] gf::Window &window)
  {

    if (toggleInventory.isActive()) {
        if (toggleInventory.isInstantaneous()) {
            toggleInventory.reset(); 
            
            std::cout << "Fermeture de l'inventaire !" << std::endl;
            m_game->popScene(); 
        }
    }
  }

  void InventoryScene::doProcessEvent(gf::Event &event)
  {
    if (event.type == gf::EventType::KeyPressed && event.key.keycode == gf::Keycode::Tab) {
        m_game->popScene();
    }
    if (event.type == gf::EventType::MouseMoved || event.type == gf::EventType::MouseButtonPressed) {
        gf::Vector2f coords = m_game->computeWindowToGameCoordinates(event.mouseCursor.coords, getHudView());
    }
    
  }

  void InventoryScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    target.setView(getHudView());
    m_inventory.render(target, states);
  }
}