#ifndef INVENTORY_H
#define INVENTORY_H

#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>
#include <gf/Widget.h>
#include <gf/Event.h>
#include "RogueCMI.h"
#include "Item.h"
#include "WorldScene.h"
#include "HeroStats.h"
#include <gf/Rect.h>



namespace rCMI {

    class Inventory {

        private: 
            bool m_isOpen = false;
            gf::WidgetContainer m_container;
            HeroStats m_stats;
            gf::TextWidget m_statsWidget;

            
            gf::SpriteWidget m_headSlot;
            gf::SpriteWidget m_legSlot;
            gf::SpriteWidget m_torsoSlot;
            gf::SpriteWidget m_handSlot;
            gf::SpriteWidget m_feetSlot;
            gf::SpriteWidget m_accessorySlot;
            std::vector<Item> m_backpack;

            Item* m_equippedHead = nullptr;
            Item* m_equippedTorso = nullptr;
            Item* m_equippedLegs = nullptr;
            Item* m_equippedHand = nullptr;
            Item* m_equippedFeet = nullptr;
            Item* m_equippedAccessory = nullptr;

            gf::RectangleShape m_background;

            gf::SpriteWidget* getWidgetByType(ItemType type) {
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

        public: 

            Inventory(RogueCMI *game) : 
            m_statsWidget("Stats", game->resources.getFont(PATH_FONT), 20),

            m_headSlot(game->resources.getTexture("slot.png"), game->resources.getTexture("slot.png"), game->resources.getTexture("slot.png")),
            m_torsoSlot(game->resources.getTexture("slot.png"), game->resources.getTexture("slot.png"), game->resources.getTexture("slot.png")),
            m_legSlot(game->resources.getTexture("slot.png"), game->resources.getTexture("slot.png"), game->resources.getTexture("slot.png")),
            m_handSlot(game->resources.getTexture("slot.png"), game->resources.getTexture("slot.png"), game->resources.getTexture("slot.png")),
            m_feetSlot(game->resources.getTexture("slot.png"), game->resources.getTexture("slot.png"), game->resources.getTexture("slot.png")),
            m_accessorySlot(game->resources.getTexture("slot.png"), game->resources.getTexture("slot.png"), game->resources.getTexture("slot.png"))

            {
                auto& font = game->resources.getFont(PATH_FONT);
                m_headSlot.setPosition({150, 50});
                m_legSlot.setPosition({150, 50});
                m_torsoSlot.setPosition({150, 50});
                m_handSlot.setPosition({150, 50});
                m_feetSlot.setPosition({150, 50});
                m_accessorySlot.setPosition({150, 50});
                
                m_container.addWidget(m_headSlot);
                m_container.addWidget(m_legSlot);
                m_container.addWidget(m_torsoSlot);
                m_container.addWidget(m_handSlot);
                m_container.addWidget(m_feetSlot);
                m_container.addWidget(m_accessorySlot);


                // background 

                m_background.setSize({400, 500});
                m_background.setColor(gf::Color::Black); 
                m_background.setPosition({50, 30});
                
                m_headSlot.setCallback([this,game]() {
                    this->onUnequip(ItemType::Head,game);
                });
                m_legSlot.setCallback([this,game]() {
                    this->onUnequip(ItemType::Legs,game);
                });
                m_torsoSlot.setCallback([this,game]() {
                    this->onUnequip(ItemType::Torso,game);
                });
                m_handSlot.setCallback([this,game]() {
                    this->onUnequip(ItemType::Hand,game);
                });
                m_feetSlot.setCallback([this,game]() {
                    this->onUnequip(ItemType::Feet,game);
                });
                m_accessorySlot.setCallback([this,game]() {
                    this->onUnequip(ItemType::Accessory,game);
                });
            }

            bool hasEquipment(ItemType type) {
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


            void setEquippedItem(ItemType type, Item* item, RogueCMI *game) {

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
                    if (item != nullptr) {
                        gf::Vector2f position(0, 0);
                        gf::Vector2f size(80, 80);
                        gf::RectF r = gf::RectF::fromPositionSize(position, size);
                        targetWidget->setDefaultSprite(*item->m_texture,r);
                    } else {
                        gf::Vector2f position(0, 0);
                        gf::Vector2f size(80, 80);
                        gf::RectF r = gf::RectF::fromPositionSize(position, size);

                        switch (type) {
                            case ItemType::Head:      targetWidget->setDefaultSprite(game->resources.getTexture("slot.png"),r); break;
                            case ItemType::Torso:     targetWidget->setDefaultSprite(game->resources.getTexture("slot.png"),r); break;
                            case ItemType::Legs:      targetWidget->setDefaultSprite(game->resources.getTexture("slot.png"),r); break;
                            case ItemType::Hand:      targetWidget->setDefaultSprite(game->resources.getTexture("slot.png"),r); break;
                            case ItemType::Feet:      targetWidget->setDefaultSprite(game->resources.getTexture("slot.png"),r); break;
                            case ItemType::Accessory: targetWidget->setDefaultSprite(game->resources.getTexture("slot.png"),r); break;
                            default: break;
                        }
                        
                        
                    }
                } 
            }

            Item* getEquippedItem(ItemType type) {
                switch (type) {
                    case ItemType::Head:      return m_equippedHead;
                    case ItemType::Torso:     return m_equippedTorso;
                    case ItemType::Legs:      return m_equippedLegs;
                    case ItemType::Hand:     return m_equippedHand;
                    case ItemType::Feet:      return m_equippedFeet;
                    case ItemType::Accessory:     return m_equippedAccessory;
                  
                    default: return nullptr;
                }
            }

            void onUnequip(ItemType type,RogueCMI *game) {

                if (hasEquipment(type)) {
                    Item *item = getEquippedItem(type);
                    //ici il faudra retirer les stats bonus qui ont été ajoutées
                    m_backpack.push_back(*item);
                    
                    setEquippedItem(type, nullptr,game);
                    updateStatsText();
                    
                    std::cout << "Objet " << item->m_name << " retiré !" << std::endl;
                }
            }


            void updateStatsText() {
                std::string str = "Force : " + std::to_string(m_stats.force) + "\n"
                                + "Santé : " + std::to_string(m_stats.health) + "\n"
                                + "Defense : " + std::to_string(m_stats.defense);
                m_statsWidget.setString(str);
                
            }

        
    };
}


#endif