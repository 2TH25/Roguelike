#ifndef RL_ITEM_SCENE_H
#define RL_ITEM_SCENE_H

#include <gf/Scene.h>
#include <gf/Font.h>
#include <gf/Shapes.h>
#include <gf/Widgets.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/WidgetContainer.h>
#include "Item.h"

namespace rCMI {
    class RogueCMI;

    class ItemScene : public gf::Scene {
    public:
        ItemScene(RogueCMI *game);

        void setItem(const Item& item, bool isEquipped, int currentChestIndex = -1, int itemIndexChest = -1);

        virtual void doProcessEvent(gf::Event &event) override;
        virtual void doRender(gf::RenderTarget &target, const gf::RenderStates &states) override;

    private:
        RogueCMI *m_game;
        
        gf::RectangleShape m_background;
        gf::Text m_nameText;
        gf::Text m_typeText;
        gf::Text m_rarityText;
        gf::Text m_descText;
        gf::Text m_statsText;
        gf::Sprite m_itemSprite;
        
        Item m_currentItem; 
        bool m_isEquipped;

        int m_currentChestIndex = -1;
        int m_itemIndexInChest = -1;

        gf::WidgetContainer m_widgets;
        gf::TextButtonWidget m_buttonEquip;
        gf::TextButtonWidget m_buttonUnequip;
        gf::TextButtonWidget m_buttonThrow;
        gf::TextButtonWidget m_buttonConsume;
        gf::TextButtonWidget m_buttonPickup;


        void onEquip();
        void onUnequip();
        void onConsume();
        void onThrow();
        
        void centerText(gf::Text& text, float y);
    };
}

#endif