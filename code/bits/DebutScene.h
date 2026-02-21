#ifndef DEBUT_SCENE_H
#define DEBUT_SCENE_H

#include <gf/Scene.h>
#include <gf/Text.h>
#include <gf/Widgets.h>
#include <gf/Shapes.h>
#include <vector>
#include <string>
namespace rCMI {
    class RogueCMI;

    class DebutScene : public gf::Scene {
    public:
        DebutScene(RogueCMI *game);

        virtual void doProcessEvent(gf::Event &event) override;
        virtual void doRender(gf::RenderTarget &target, const gf::RenderStates &states) override;

    private:
        RogueCMI *m_game;
        gf::RectangleShape m_background;
        gf::Text m_titre;
        gf::Text m_message;
        gf::Text m_infoCommandes;
        
        gf::TextWidget m_boutonOk;
        gf::RectangleShape m_boutonBackground;
    };
}

#endif