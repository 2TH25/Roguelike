#include "ItemScene.h"
#include <gf/Coordinates.h>
#include "RogueCMI.h"
#include "Item.h"
#include <thread>
#include <chrono>


namespace rCMI {


    std::string itemTypeToString(ItemType type) {
        switch(type) {
            case ItemType::Consumable: return "Consommable";
            case ItemType::Weapon:     return "Arme";
            case ItemType::Head:      return "Casque";
            case ItemType::Torso:     return "Plastron";
            case ItemType::Legs:      return "Jambiere";
            case ItemType::Boots:     return "Bottes";
            case ItemType::Hand:     return "Gants";
            case ItemType::Accessory:     return "Bracelet";
            case ItemType::Misc:     return "Divers";
            case ItemType::Bow:     return "Arc";
            case ItemType::Arrow:     return "Flèche";

            default: break;
        }
        return "Inconnu";
    }

    std::string itemRarityToString(Item::Rarity rarity) {
        switch(rarity) {
            case Item::Rarity::Legendary: return "Légendaire";
            case Item::Rarity::Epic: return "Épique";
            case Item::Rarity::Rare: return "Rare";
            case Item::Rarity::Uncommon: return "Peu Commun";
            case Item::Rarity::Common: return "Commun";

            default: break;
        }
        return "Inconnu";
    }



     ItemScene::ItemScene(RogueCMI *game)
      : gf::Scene(gf::Vector2f(1200, 800))
      , m_game(game)
      , m_nameText("", game->resources.getFont(PATH_FONT), 30)
      , m_typeText("", game->resources.getFont(PATH_FONT), 20)
      , m_rarityText("", game->resources.getFont(PATH_FONT), 20)
      , m_descText("", game->resources.getFont(PATH_FONT), 18)
      , m_statsText("", game->resources.getFont(PATH_FONT), 18)
      , m_buttonEquip("Equiper", game->resources.getFont(PATH_FONT))
      , m_buttonUnequip("Desequiper", game->resources.getFont(PATH_FONT))
      , m_buttonThrow("Jeter", game->resources.getFont(PATH_FONT))
      , m_buttonConsume("Consommer", game->resources.getFont(PATH_FONT))
      , m_buttonPickup("Ramasser", game->resources.getFont(PATH_FONT))
    {
        setClearColor(gf::Color::Transparent);

        m_background.setSize({400.0f, 500.0f});
        m_background.setTexture(game->resources.getTexture("backgroundItem.png"));
        m_background.setAnchor(gf::Anchor::Center);
        m_background.setPosition({600.0f, 400.0f});
        m_background.setOutlineColor(gf::Color::Black);
        m_background.setOutlineThickness(2.0f);
		

        m_typeText.setColor(gf::Color::Black);
        m_descText.setParagraphWidth(350.0f);
        m_descText.setAlignment(gf::Alignment::Center);
        

        m_buttonEquip.setCallback([this]() { onEquip(); });
        m_buttonUnequip.setCallback([this]() { onUnequip(); });
        m_buttonConsume.setCallback([this]() { onConsume(); });
        m_buttonThrow.setCallback([this]() { onThrow(); });
        m_buttonPickup.setCallback([this]() { onEquip(); });

        m_widgets.addWidget(m_buttonEquip);
        m_widgets.addWidget(m_buttonUnequip);
        m_widgets.addWidget(m_buttonConsume);
        m_widgets.addWidget(m_buttonThrow);
        m_widgets.addWidget(m_buttonPickup);
    }



    void ItemScene::setItem(const Item& item, bool isEquipped, int chestIndex, int itemIndex) {
        m_currentItem = item;
        m_isEquipped = isEquipped;
        m_currentChestIndex = chestIndex;
        m_itemIndexInChest = itemIndex;


        switch (item.m_rarity) {
            case Item::Rarity::Common:
                m_nameText.setColor(gf::Color::White);
                m_rarityText.setColor(gf::Color::White);  // Blanc pour le commun
                break;
            case Item::Rarity::Uncommon:
                m_nameText.setColor(gf::Color::Green);
                m_rarityText.setColor(gf::Color::Green); // Vert pour l'insolite
                break;
            case Item::Rarity::Rare:
                m_nameText.setColor(gf::Color::Azure);
                m_rarityText.setColor(gf::Color::Azure); // Bleu/Azure pour le rare
                break;
            case Item::Rarity::Epic:
                m_nameText.setColor(gf::Color::fromRgba32(0xA335EEFF));
                m_rarityText.setColor(gf::Color::fromRgba32(0xA335EEFF)); // Violet épique
                break;
            case Item::Rarity::Legendary:
                m_nameText.setColor(gf::Color::Red);
                m_rarityText.setColor(gf::Color::Red); // Rouge pour le légendaire
                break;
            default:
                m_nameText.setColor(gf::Color::White);
                m_rarityText.setColor(gf::Color::White);
                break;
        }


        m_nameText.setString(item.m_name);
        centerText(m_nameText, 200.0f);

        m_typeText.setString("Type : " + itemTypeToString(item.m_type)); 
        centerText(m_typeText, 270.0f);

        m_rarityText.setString("Rareté : " + itemRarityToString(item.m_rarity)); 
        centerText(m_rarityText, 240.0f);

        m_descText.setString(item.m_description.empty() ? "Aucune description." : item.m_description);
        m_descText.setAnchor(gf::Anchor::Center);
        m_descText.setPosition({600.0f, 420.0f});

        std::string statsStr = "";
        if (item.m_stat.getPower() > 0) statsStr += "Atk: +" + std::to_string(item.m_stat.getPower()) + " ";
        if (item.m_stat.getHealth() > 0) statsStr += "Hp: +" + std::to_string(item.m_stat.getHealth()) + " ";
        if (item.m_stat.getDefense() > 0) statsStr += "Def: +" + std::to_string(item.m_stat.getDefense()) + " ";
        m_statsText.setString(statsStr);
        if (statsStr.empty()) {
            m_statsText.setPosition({-1000.0f, -1000.0f}); 
        } else {
            centerText(m_statsText, 520.0f);
        }

        std::string textureKey = item.m_id + ".png"; 
        const gf::Texture& tex = m_game->resources.getTexture(textureKey);
        
        if (m_currentItem.m_texture != nullptr) {
            m_itemSprite.setTexture(*m_currentItem.m_texture); 
            
            m_itemSprite.setAnchor(gf::Anchor::Center);
            m_itemSprite.setPosition({600.0f, 330.0f});
            m_itemSprite.setScale(static_cast<float>(TileSize) / 640.0f); 
            m_itemSprite.setColor(gf::Color::White);
        }
        
        // on va tout désactiver par défaut
        m_buttonEquip.setDisabled();
        m_buttonUnequip.setDisabled();
        m_buttonConsume.setDisabled();
        m_buttonThrow.setDisabled();
        m_buttonPickup.setDisabled();

        float btnY = 580.0f;
        float centerX = 600.0f;

        if (m_currentChestIndex != -1) { // dans un coffre
            m_buttonPickup.setSelected(); // On active Pickup
            m_buttonPickup.setPosition({600.0f, btnY});
        } else { // pas dans un coffre

            if (m_currentItem.m_type == ItemType::Arrow) {
                m_buttonThrow.setSelected();
                m_buttonThrow.setPosition({centerX, btnY}); 
            }

            else if (m_currentItem.m_type == ItemType::Consumable) {
                m_buttonConsume.setSelected();
                m_buttonThrow.setSelected();
                
                m_buttonConsume.setPosition({centerX - 120, btnY});
                m_buttonThrow.setPosition({centerX + 80, btnY});
            } 
            else {
                if (m_isEquipped) {
                    m_buttonUnequip.setSelected();
                    m_buttonThrow.setSelected();
                    m_buttonUnequip.setPosition({centerX - 150, btnY});
                    m_buttonThrow.setPosition({centerX + 80, btnY});
                } else {
                    m_buttonEquip.setSelected();
                    m_buttonThrow.setSelected();
                    m_buttonEquip.setPosition({centerX - 150, btnY});
                    m_buttonThrow.setPosition({centerX + 80, btnY});
                }
            }
        }        
    }

    void ItemScene::centerText(gf::Text& text, float y) {
        text.setAnchor(gf::Anchor::TopCenter);
        text.setPosition({600.0f, y});
    }


    void ItemScene::onEquip() {
        if (m_currentChestIndex != -1) { 
            bool success = m_game->m_InventoryScene->m_inventory.addItemFromChest(m_currentChestIndex, m_itemIndexInChest, m_game);
            
            if (success) {
                bool isChestEmpty = m_game->m_ChestScene.updateChestAfterPickup();
                m_game->popScene();
                
                if (isChestEmpty) {
                    m_game->popScene();
                }
            }
        } 
        else { 
            Item oldItem = m_game->m_InventoryScene->m_inventory.setEquippedItem(m_currentItem.m_type, &m_currentItem, m_game);
            m_game->m_InventoryScene->m_inventory.removeItemFromBackpack(m_currentItem, m_game);
            if (!oldItem.m_name.empty()) {
                m_game->m_InventoryScene->m_inventory.addItemToBackpack(oldItem, m_game);
            }
            m_game->popScene();
        }
    }


    void ItemScene::onUnequip() {
        this->m_game->m_InventoryScene->m_inventory.onUnequip(m_currentItem.m_type, this->m_game);
        this->m_game->popScene();
    }

    void ItemScene::onConsume() {
        this->m_game->m_InventoryScene->m_inventory.consumeItem(m_currentItem, this->m_game);
        m_game->popScene();
    }

    void ItemScene::onThrow() {
        if (m_isEquipped) {
            m_game->m_InventoryScene->m_inventory.onUnequip(m_currentItem.m_type, m_game);
        } 
        
        m_game->m_InventoryScene->m_inventory.removeItemFromBackpack(m_currentItem, m_game);
        m_game->popScene();
    }



    void ItemScene::doProcessEvent(gf::Event &event) {
        if(!isActive()) {
            return;
        }

        if (event.type == gf::EventType::MouseButtonPressed && event.mouseButton.button == gf::MouseButton::Left) {
            gf::Vector2f mouseCoords = event.mouseButton.coords;

            gf::Vector2f localMousePos = gf::transform(m_background.getInverseTransform(), mouseCoords);
            if (!m_background.getLocalBounds().contains(localMousePos)) {
                m_game->popScene();
                return;
            }

            if (!m_buttonEquip.isDisabled() && m_buttonEquip.contains(mouseCoords)) {
                onEquip();
                return;
            }
            if (!m_buttonUnequip.isDisabled() && m_buttonUnequip.contains(mouseCoords)) {
                onUnequip();
                return;
            }
            if (!m_buttonConsume.isDisabled() && m_buttonConsume.contains(mouseCoords)) {
                onConsume();
                return;
            }
            if (!m_buttonThrow.isDisabled() && m_buttonThrow.contains(mouseCoords)) {
                onThrow();
                return;
            }
            if (!m_buttonPickup.isDisabled() && m_buttonPickup.contains(mouseCoords)) {
                onEquip();
                return;
            }
        }
        
        if (event.type == gf::EventType::MouseButtonPressed && event.mouseButton.button == gf::MouseButton::Right) {
            m_game->popScene();
            return;
        }
        if (event.type == gf::EventType::KeyPressed && event.key.keycode == gf::Keycode::Escape) {
            m_game->popScene();
            event.type = gf::EventType::MouseMoved;
            return;
        }
    }


    void ItemScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states) {
        target.draw(m_background, states);

        if (m_currentItem.m_texture != nullptr) {
            target.draw(m_itemSprite, states);
        }

        target.draw(m_nameText, states);
        target.draw(m_typeText, states);
        target.draw(m_rarityText, states);
        target.draw(m_descText, states);
        target.draw(m_statsText, states);
        
        if (!m_buttonEquip.isDisabled()) m_buttonEquip.draw(target, states);
        if (!m_buttonUnequip.isDisabled()) m_buttonUnequip.draw(target, states);
        if (!m_buttonConsume.isDisabled()) m_buttonConsume.draw(target, states);
        if (!m_buttonThrow.isDisabled()) m_buttonThrow.draw(target, states);
        if (!m_buttonPickup.isDisabled()) m_buttonPickup.draw(target, states);
    }
}