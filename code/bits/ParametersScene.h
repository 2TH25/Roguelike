#ifndef RL_PARAMETERS_SCENE_H
#define RL_PARAMETERS_SCENE_H

#include <gf/Scene.h>
#include <gf/Text.h>
#include <gf/Shapes.h>
#include <vector>
#include <string>

namespace rCMI {
    class RogueCMI;

    class ParametersScene : public gf::Scene {
    public:
        ParametersScene(RogueCMI *game);

        virtual void doProcessEvent(gf::Event &event) override;
        virtual void doRender(gf::RenderTarget &target, const gf::RenderStates &states) override;

    private:
        RogueCMI *m_game;
        gf::Text m_helpCommandsText;
        gf::RectangleShape m_background;
        std::string helpCommandsString;
        
        
    };
}

#endif // RL_PARAMETERS_SCENE_H