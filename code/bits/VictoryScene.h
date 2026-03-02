#ifndef RL_VICTORY_SCENE_H
#define RL_VICTORY_SCENE_H

#include <gf/Scene.h>
#include <gf/Text.h>
#include <gf/Widgets.h>
#include <gf/Shapes.h>
#include <gf/Coordinates.h> 
#include <gf/WidgetContainer.h>

namespace rCMI {
    class RogueCMI;

    class VictoryScene : public gf::Scene {
    public:
        VictoryScene(RogueCMI *game);
        
        // Nouvelle fonction avec tes 3 paramètres
        void setFinalStats(int score, int kills, int highest_level);

    private:
        void doRender(gf::RenderTarget &target, const gf::RenderStates &states) override;
        void doProcessEvent(gf::Event &event) override;

        RogueCMI *m_game = nullptr;
        gf::Font &font;
        gf::WidgetContainer widgets;
        
        gf::TextButtonWidget m_quit;
        // Le bouton m_continue a été retiré selon ton nouveau design
        
        gf::Text m_title;
        gf::Text m_messageText;
        gf::Text m_statsText; // Remplacement de m_scoreText
        gf::RectangleShape m_background;
    };
}

#endif