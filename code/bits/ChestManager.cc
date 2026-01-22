#include "ChestManager.h"
#include "RogueCMI.h"

namespace rCMI {

    void ChestManager::update(RogueCMI *game) {
        gf::Vector2f heroPos = game->m_WorldScene.m_world_entity.hero().getExistence().getPosition();
        gf::Vector2i heroGridPos = { (int)heroPos.x, (int)heroPos.y };

        for (auto &chest : m_chests) {
            if (!chest.isOpen && chest.gridPos == heroGridPos) {
                openChest(chest, game);
            }
        }
    }

    void ChestManager::openChest(Chest &chest, RogueCMI *game) {
        chest.isOpen = true;
        
        chest.sprite.setTexture(game->resources.getTexture("ChestOpen.png"));
        
        std::cout << "Vous avez ouvert un coffre contenant : " << chest.content.m_name << std::endl;
        game->m_WorldScene.m_world_entity.m_inventory.addItemFromChest(chest.content, game);
    }

    void ChestManager::spawnChest(gf::Vector2i pos, RogueCMI *game) {
        Chest c;
        c.gridPos = pos;
        c.content = Item::generateRandomItem(game);
        c.isOpen = false;
        
        c.sprite.setTexture(game->resources.getTexture("ChestClosed.png"));
        c.sprite.setPosition({ (pos.x * TileSize) + TileSize/2.0f, (pos.y * TileSize) + TileSize/2.0f });
        
        gf::Vector2f size = c.sprite.getLocalBounds().getSize();
        c.sprite.setOrigin(size / 2.0f);
        
        m_chests.push_back(c);
    }

    void ChestManager::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    for (auto &chest : m_chests) {
        target.draw(chest.sprite, states);
    }
}

}