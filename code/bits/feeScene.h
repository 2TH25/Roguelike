#ifndef RL_FEE_SCENE_H
#define RL_FEE_SCENE_H

#include <gf/Scene.h>
#include <gf/Text.h>
#include <gf/Shapes.h>
#include <vector>
#include <string>

namespace rCMI {
    class RogueCMI;

    class FeeScene : public gf::Scene {
    public:
        FeeScene(RogueCMI *game);

        void setDialogue(const std::vector<std::string>& lines);

        virtual void doProcessEvent(gf::Event &event) override;
        virtual void doRender(gf::RenderTarget &target, const gf::RenderStates &states) override;

    private:
        RogueCMI *m_game;
        
        gf::RectangleShape m_box;      
        gf::Text m_text;              
        gf::Text m_clickHint;        

        std::vector<std::string> m_dialogueLines;
        size_t m_currentLine;

        void updateText();
    };
}

#endif // RL_FEE_SCENE_H