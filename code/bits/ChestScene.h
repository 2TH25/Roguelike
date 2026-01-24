#ifndef RL_CHEST_SCENE_H
#define RL_CHEST_SCENE_H

#include <gf/Scene.h>
#include <gf/Font.h>
#include <gf/Shapes.h>
#include <gf/Widgets.h>
#include <gf/Sprite.h>
#include <vector>
#include "ChestManager.h"

namespace rCMI {
    class RogueCMI;
    class Chest;

    class ChestScene : public gf::Scene {
    public:
        ChestScene(RogueCMI *game);
        void setLoot(Item item);
        virtual void doProcessEvent(gf::Event &event) override;
        virtual void doRender(gf::RenderTarget &target, const gf::RenderStates &states) override;
        void setLoots(Chest& chest);
        Chest* m_currentChest = nullptr;

    private:
        RogueCMI *m_game;
        std::vector<Item> m_loots;
        gf::RectangleShape m_background;
        gf::TextWidget m_text;
        
        std::vector<gf::Sprite> m_contentSprites;
    };
}

#endif