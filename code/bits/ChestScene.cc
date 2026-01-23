#include "ChestScene.h"
#include <gf/Coordinates.h>
#include "RogueCMI.h"

namespace rCMI
{
    void ChestScene::doProcessEvent(gf::Event &event) {
        if (event.type == gf::EventType::KeyPressed) {
            if (event.key.keycode == gf::Keycode::Space || event.key.keycode == gf::Keycode::Enter) {
                if (m_game->m_InventoryScene->m_inventory.addItemToBackpack(m_loot, m_game)) {
                    m_game->popScene();
                } 
            }
            if (event.key.keycode == gf::Keycode::Escape) {
                m_game->popScene();
            }
        }
    }

    void ChestScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states) {
        gf::RectangleShape overlay(target.getCanvasSize());
        overlay.setColor(gf::Color::fromRgba32(0, 0, 0, 150));
        target.draw(overlay);

        target.draw(m_background, states);
    }
}