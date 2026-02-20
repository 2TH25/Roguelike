#include "ChestManager.h"
#include "RogueCMI.h"
#include "InventoryScene.h"

namespace rCMI {

   void ChestManager::update(RogueCMI *game) {
        gf::Vector2f heroPos = game->m_WorldScene.m_world_entity.hero().getExistence().getPosition();
        gf::Vector2i heroGridPos = { (int)heroPos.x, (int)heroPos.y };

        for (int i = 0; i < (int)m_chests.size(); ++i) {
            if (!m_chests[i].isOpen && m_chests[i].gridPos == heroGridPos) {
                openChest(i, game);
            }
        }
    }

    void ChestManager::openChest(int chestIndex, RogueCMI *game) {
        Chest &chest = m_chests[chestIndex];

        if(!chest.distributeScore) {
            game->m_WorldScene.m_world_entity.hero().getStat().score += 20;
            chest.distributeScore = true;
        }
        
        game->m_ChestScene.setLoots(chest, chestIndex);
        game->m_WorldScene.m_isActivateChest = true;
        game->pushScene(game->m_ChestScene);
        std::cout << "Ouverture du coffre index : " << chestIndex << std::endl;
    }

    void ChestManager::spawnChest(gf::Vector2i pos, RogueCMI *game) {
        Chest c;
        c.gridPos = pos;
        int roll = rand() % 100;
        int itemInChestCount = 0;
        if(roll<40) {itemInChestCount=1;}
        else if(roll<70) {itemInChestCount=2;}
        else if(roll<90) {itemInChestCount=3;}
        else {itemInChestCount=4;}
        for(int i=0;i<itemInChestCount;i++){
            c.content.push_back(Item::generateRandomItem(game));
        }
        
        c.isOpen = false;
        
        c.sprite.setTexture(game->resources.getTexture("Coffre.png"));
        float scale = static_cast<float>(TileSize) / 640.0f;
        c.sprite.setScale(scale);
        c.sprite.setPosition({ (float)pos.x * TileSize, (float)pos.y * TileSize });
        
        gf::Vector2f size = c.sprite.getLocalBounds().getSize();        
        m_chests.push_back(c);
    }

    void ChestManager::render(gf::RenderTarget &target, const gf::RenderStates &states, Map &map) {
    for (auto &chest : m_chests) {
        if (map.isInFieldOfVision(chest.gridPos)) {
            target.draw(chest.sprite, states);
        }
    }
}

    int ChestManager::isChestOnTile(RogueCMI *game) {
        gf::Vector2f heroPos = game->m_WorldScene.m_world_entity.hero().getExistence().getPosition();
        gf::Vector2i heroGridPos = { static_cast<int>(heroPos.x), static_cast<int>(heroPos.y) };

        for (int i = 0; i < (int)m_chests.size(); i++) {
            if (heroGridPos.x == m_chests[i].gridPos.x && 
                heroGridPos.y == m_chests[i].gridPos.y) {
                if (!m_chests[i].isOpen) {
                    return i;
                }
            }
        }

        return -1;
    }
    void ChestManager::clear() {
        m_chests.clear();
    }

}