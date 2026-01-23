#ifndef RL_CHEST_SCENE_H
#define RL_CHEST_SCENE_H

#include <gf/Scene.h>
#include <gf/Font.h>
#include "ChestManager.h"

namespace rCMI {
    class ChestScene : public gf::Scene {
    public:
        ChestScene(RogueCMI *game);
        void setLoot(Item item);
        virtual void doProcessEvent(gf::Event &event) override;
        virtual void doRender(gf::RenderTarget &target, const gf::RenderStates &states) override;

    private:
        RogueCMI *m_game;
        Item m_loot;
        gf::RectangleShape m_background;
        gf::SpriteWidget m_itemWidget;
        gf::TextWidget m_text;
    };
}


#endif
