#include "ChestScene.h"
#include <gf/Coordinates.h>
#include "RogueCMI.h"
#include "Chest.h"
#include <algorithm>
#include <random>
#include <thread>
#include <chrono>

namespace rCMI
{

    ChestScene::ChestScene(RogueCMI *game)
    : gf::Scene(gf::Vector2f(1200, 800)) 
    , m_game(game)
    , m_background({600.0f, 400.0f})
    , m_text("", game->resources.getFont(PATH_FONT), 20)
{
    setClearColor(gf::Color::Transparent);
    m_background.setTexture(game->resources.getTexture("backgroundChest.png"));
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
                m_game->popScene();
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                m_game->m_WorldScene.m_isActivateChest = false;
                m_game->popScene();
            }
        }
    }

    void ChestScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states) {
        const gf::Vector2f vSize = target.getView().getSize();
        float chestWidth = vSize.x * 0.40f; 
        float scaleBG = chestWidth / 600.0f;
        
        gf::Vector2f bgPos = { vSize.x * 0.15f, vSize.y * 0.15f };
        m_background.setPosition(bgPos);
        m_background.setScale(scaleBG);
        target.draw(m_background, states);

    
        float slotW = 78.0f * scaleBG; 
        float slotH = 76.0f * scaleBG;
        float padX = 10.0f * scaleBG; 
        float padY = 10.0f * scaleBG;

        float firstCenterX = bgPos.x + (76.0f * scaleBG);
        float firstCenterY = bgPos.y + (76.0f * scaleBG);


        float scaleItem = (64.0f * scaleBG) / 640.0f; 

        for (size_t i = 0; i < m_contentSprites.size(); ++i) {
            int slotIndex = m_itemSlotIndices[i]; 
            int col = slotIndex % 6;
            int row = slotIndex / 6;

            float centerX = firstCenterX + col * (slotW + padX);
            float centerY = firstCenterY + row * (slotH + padY);

            m_contentSprites[i].setScale(scaleItem);
            m_contentSprites[i].setPosition({centerX, centerY});
            
            target.draw(m_contentSprites[i], states);
        }
    }

    void ChestScene::setLoots(Chest& chest, int indexChest) {
        m_currentChestIndex = indexChest;
        m_currentChest = &chest;
        m_loots = chest.content; 
        m_contentSprites.clear();
        m_itemSlotIndices.clear();

        if (m_currentChest->itemSlotsPosition.empty() && !m_loots.empty()) {
            std::vector<int> availableSlots;
            for (int i = 0; i < 24; ++i) availableSlots.push_back(i);

            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(availableSlots.begin(), availableSlots.end(), g);

            for (size_t i = 0; i < m_loots.size(); ++i) {
                m_currentChest->itemSlotsPosition.push_back(availableSlots[i]);
            }
        }

        m_itemSlotIndices = m_currentChest->itemSlotsPosition;
        

        for (size_t i = 0; i < m_loots.size(); ++i) {
            if (m_loots[i].m_texture != nullptr) {
                gf::Sprite sprite;
                sprite.setTexture(*(m_loots[i].m_texture));
                gf::Vector2f textureSize = m_loots[i].m_texture->getSize();
                sprite.setTextureRect(gf::RectF::fromSize(m_loots[i].m_texture->getSize()));
                sprite.setOrigin(textureSize / 2.0f);
                m_contentSprites.push_back(sprite);
            }
        }
    }
}