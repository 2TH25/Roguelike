#ifndef RL_VICTORY_SCENE_H
#define RL_VICTORY_SCENE_H

#include <gf/Scene.h>
#include <gf/Text.h>
#include <gf/Widgets.h>
#include <gf/Shapes.h>

namespace rCMI {
    class RogueCMI;

    class VictoryScene : public gf::Scene {
    public:
        VictoryScene(RogueCMI *game);
        void setFinalScore(int score);

        virtual void doProcessEvent(gf::Event &event) override;
        virtual void doRender(gf::RenderTarget &target, const gf::RenderStates &states) override;

    private:
        RogueCMI *m_game;
        gf::RectangleShape m_background;
        gf::Text m_title;
        gf::Text m_messageText;
        gf::Text m_scoreText;
        
        gf::TextWidget m_boutonQuit;
        gf::RectangleShape m_boutonBackground;
    };
}

#endif