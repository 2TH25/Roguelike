#include "ChestScene.h"
#include <gf/Coordinates.h>
#include "RogueCMI.h"
#include "Chest.h"

namespace rCMI
{

    ChestScene::ChestScene(RogueCMI *game)
    : gf::Scene(gf::Vector2f(1200, 800)) 
    , m_game(game)
    , m_background({600.0f, 400.0f})
    , m_text("", game->resources.getFont(PATH_FONT), 20)
{
    setClearColor(gf::Color::Transparent);
    m_background.setColor(gf::Color::fromRgba32(50, 50, 50, 200));
    m_background.setAnchor(gf::Anchor::Center);
    
    m_background.setPosition(gf::Vector2f(600, 400)); 
}


    void ChestScene::doProcessEvent(gf::Event &event) {
        if (event.type == gf::EventType::MouseButtonPressed && event.mouseButton.button == gf::MouseButton::Left) {
            gf::Vector2f mouseCoords = event.mouseButton.coords;

            for (size_t i = 0; i < m_contentSprites.size(); ++i) {
                gf::Matrix3f inverseMatrix = m_contentSprites[i].getInverseTransform();
                gf::Vector2f localMousePos = gf::transform(inverseMatrix, mouseCoords);

                if (m_contentSprites[i].getLocalBounds().contains(localMousePos)) {
                    m_isItemSceneOpen = true;
                    m_game->m_ItemScene.setItem(m_loots[i], false, m_currentChestIndex, i);
                    m_game->pushScene(m_game->m_ItemScene);
                    return;
                }
            }
        }

        if (event.type == gf::EventType::KeyPressed && event.key.keycode == gf::Keycode::Escape) {
            if (m_game->m_WorldScene.m_isActivateInventory) {
                return; 
            }
            m_game->m_WorldScene.m_isActivateChest = false;
            m_game->popScene();
            event.type = gf::EventType::MouseMoved;
            return;
        }
    }


    void ChestScene::updateChestAfterPickup() {
        m_isItemSceneOpen = false;

        if (m_currentChest) {
            m_loots = m_currentChest->content;
            setLoots(*m_currentChest, m_currentChestIndex);

            if (m_loots.empty()) {
                m_currentChest->isOpen = true;
                m_currentChest->sprite.setTexture(m_game->resources.getTexture("CoffreOuvert.png"));
                m_game->m_WorldScene.m_isActivateChest = false;
                m_game->popScene();
                m_game->popScene();
            }
        }
    }

    void ChestScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states) {
        target.draw(m_background, states);
        for (auto &sprite : m_contentSprites) {
            target.draw(sprite, states);
        }
    }


    void ChestScene::setLoots(Chest& chest, int indexChest) {
        m_currentChestIndex = indexChest;
        m_currentChest = &chest;
        m_loots = chest.content; 
        m_contentSprites.clear();

        float scale = static_cast<float>(TileSize) / 640.0f;

        for (size_t i = 0; i < m_loots.size(); ++i) {
            gf::Sprite sprite;
            
            if (m_loots[i].m_texture != nullptr) {
                sprite.setTexture(*(m_loots[i].m_texture));
                sprite.setTextureRect(gf::RectF::fromSize(m_loots[i].m_texture->getSize()));
                sprite.setScale(scale);
                
                float x = 400.0f + (i * 90.0f);
                float y = 300.0f;
                sprite.setPosition({x, y});
                
                m_contentSprites.push_back(sprite);
            }
        }
    }
}