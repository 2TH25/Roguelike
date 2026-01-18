#include "Inventory.h"
#include "RogueCMI.h"

namespace rCMI {

    Inventory::Inventory(RogueCMI *game) : 
    
        m_statsWidget("Stats\n", game->resources.getFont(PATH_FONT), 20),
        m_headSlot(game->resources.getTexture("Casque.jpg"), game->resources.getTexture("Casque.jpg"), game->resources.getTexture("Casque.jpg")),
        m_torsoSlot(game->resources.getTexture("Plastron.png"), game->resources.getTexture("Plastron.png"), game->resources.getTexture("Plastron.png")),
        m_legSlot(game->resources.getTexture("Jambieres.jpg"), game->resources.getTexture("Jambieres.jpg"), game->resources.getTexture("Jambieres.jpg")),
        m_handSlot(game->resources.getTexture("Gants.jpg"), game->resources.getTexture("Gants.jpg"), game->resources.getTexture("Gants.jpg")),
        m_feetSlot(game->resources.getTexture("Bottes.jpg"), game->resources.getTexture("Bottes.jpg"), game->resources.getTexture("Bottes.jpg")),
        m_accessorySlot(game->resources.getTexture("accessoire.jpg"), game->resources.getTexture("accessoire.jpg"), game->resources.getTexture("accessoire.jpg"))
    {
        m_headSlot.setPosition({220, 110});
        m_legSlot.setPosition({500, 300});
        m_torsoSlot.setPosition({500, 230});
        m_handSlot.setPosition({220, 200});
        m_feetSlot.setPosition({500, 380});
        m_accessorySlot.setPosition({220, 300});

        m_heroSprite.setTexture(game->resources.getTexture("perso100.png"));
        m_heroSprite.setPosition({250, 120}); 
        m_heroSprite.setScale({3.0f, 3.0f});
        
        m_container.addWidget(m_headSlot);
        m_container.addWidget(m_legSlot);
        m_container.addWidget(m_torsoSlot);
        m_container.addWidget(m_handSlot);
        m_container.addWidget(m_feetSlot);
        m_container.addWidget(m_accessorySlot);

        m_background.setSize({1200, 700});
        m_background.setColor(gf::Color::White); 
        m_background.setPosition({200, 100});

        m_statsWidget.setPosition({800, 200});
        m_statsWidget.setDefaultTextColor(gf::Color::Black);

        updateStatsText();
        
        m_headSlot.setCallback([this, game]() {
            this->onUnequip(ItemType::Head, game);
        });
        m_legSlot.setCallback([this, game]() {
            this->onUnequip(ItemType::Legs, game);
        });
        m_torsoSlot.setCallback([this, game]() {
            this->onUnequip(ItemType::Torso, game);
        });
        m_handSlot.setCallback([this, game]() {
            this->onUnequip(ItemType::Hand, game);
        });
        m_feetSlot.setCallback([this, game]() {
            this->onUnequip(ItemType::Feet, game);
        });
        m_accessorySlot.setCallback([this, game]() {
            this->onUnequip(ItemType::Accessory, game);
        });
    }

    gf::SpriteWidget* Inventory::getWidgetByType(ItemType type) {
        switch (type) {
            case ItemType::Head:      return &m_headSlot;
            case ItemType::Torso:     return &m_torsoSlot;
            case ItemType::Legs:      return &m_legSlot;
            case ItemType::Hand:      return &m_handSlot;
            case ItemType::Feet:      return &m_feetSlot;
            case ItemType::Accessory: return &m_accessorySlot;
            default:                  return nullptr; 
        }
    }

    bool Inventory::hasEquipment(ItemType type) {
        switch (type) {
            case ItemType::Head:      return m_equippedHead != nullptr;
            case ItemType::Torso:     return m_equippedTorso != nullptr;
            case ItemType::Legs:      return m_equippedLegs != nullptr;
            case ItemType::Hand:      return m_equippedHand != nullptr;
            case ItemType::Feet:      return m_equippedFeet != nullptr;
            case ItemType::Accessory: return m_equippedAccessory != nullptr;
            default: return false;
        }
    }

    void Inventory::setEquippedItem(ItemType type, Item* item, RogueCMI *game) {
        switch (type) {
            case ItemType::Head:      m_equippedHead = item; break;
            case ItemType::Torso:     m_equippedTorso = item; break;
            case ItemType::Legs:      m_equippedLegs = item; break;
            case ItemType::Hand:      m_equippedHand = item; break;
            case ItemType::Feet:      m_equippedFeet = item; break;
            case ItemType::Accessory: m_equippedAccessory = item; break;
            default: break;
        }

        gf::SpriteWidget* targetWidget = getWidgetByType(type);
        
        if (targetWidget != nullptr) {
            gf::RectF r = gf::RectF::fromPositionSize({0, 0}, {80, 80});
            if (item != nullptr) {
                targetWidget->setDefaultSprite(*item->m_texture, r);
            } else {
                switch (type) {
                    case ItemType::Head:      targetWidget->setDefaultSprite(game->resources.getTexture("Casque.jpg"),r); break;
                    case ItemType::Torso:     targetWidget->setDefaultSprite(game->resources.getTexture("Plastron.png"),r); break;
                    case ItemType::Legs:      targetWidget->setDefaultSprite(game->resources.getTexture("Jambieres.jpg"),r); break;
                    case ItemType::Hand:      targetWidget->setDefaultSprite(game->resources.getTexture("Gants.jpg"),r); break;
                    case ItemType::Feet:      targetWidget->setDefaultSprite(game->resources.getTexture("Bottes.jpg"),r); break;
                    case ItemType::Accessory: targetWidget->setDefaultSprite(game->resources.getTexture("accessoire.jpg"),r); break;
                    default: break;
                }
            }
        } 

        updateStatsText();
    }

    Item* Inventory::getEquippedItem(ItemType type) {
        switch (type) {
            case ItemType::Head:      return m_equippedHead;
            case ItemType::Torso:     return m_equippedTorso;
            case ItemType::Legs:      return m_equippedLegs;
            case ItemType::Hand:      return m_equippedHand;
            case ItemType::Feet:      return m_equippedFeet;
            case ItemType::Accessory: return m_equippedAccessory;
            default: return nullptr;
        }
    }

    void Inventory::onUnequip(ItemType type, RogueCMI *game) {
        if (hasEquipment(type)) {
            Item *item = getEquippedItem(type);
            m_backpack.push_back(*item);
            setEquippedItem(type, nullptr, game);
            updateStatsText();
            std::cout << "Objet " << item->m_name << " retiré !" << std::endl;
        }
    }

    void Inventory::updateStatsText() {
        std::string str = "Force : " + std::to_string(m_stats.getPower()) + "\n"
                        + "Santé : " + std::to_string(m_stats.getMaxHealth()) + "\n"
                        + "Defense : " + std::to_string(m_stats.getDefense());
        m_statsWidget.setString(str);
    }

    void Inventory::render(gf::RenderTarget& target, const gf::RenderStates& states) {
        target.draw(m_background, states);
        target.draw(m_heroSprite, states);
        m_container.render(target, states);
        m_statsWidget.draw(target, states);
    }
}