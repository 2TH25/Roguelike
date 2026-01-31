#include "feeScene.h"
#include "RogueCMI.h"
#include "GameData.h"

namespace rCMI {

    FeeScene::FeeScene(RogueCMI *game)
    : gf::Scene(gf::Vector2f(600, 400)) 
    , m_game(game)
    , m_currentLine(0)
    {
        setClearColor(gf::Color::Transparent); 

        m_box.setSize({1000, 200});
        m_box.setOutlineColor(gf::Color::Rose);
        m_box.setOutlineThickness(2);
        m_box.setAnchor(gf::Anchor::BottomCenter);
        m_box.setPosition({600, 750});

        m_text.setFont(m_game->resources.getFont(PATH_FONT));
        m_text.setCharacterSize(30);
        m_text.setAnchor(gf::Anchor::TopLeft);
        m_text.setPosition({120, 580});
        m_text.setParagraphWidth(960);

        m_clickHint.setFont(m_game->resources.getFont(PATH_FONT));
        m_clickHint.setString("(Cliquez pour continuer)");
        m_clickHint.setCharacterSize(15);
        m_clickHint.setAnchor(gf::Anchor::BottomRight);
        m_clickHint.setPosition({1080, 740});
    }

    void FeeScene::setDialogue(const std::vector<std::string>& lines) {
        m_dialogueLines = lines;
        m_currentLine = 0;
        updateText();
    }

    void FeeScene::updateText() {
        if (m_currentLine < m_dialogueLines.size()) {
            m_text.setString(m_dialogueLines[m_currentLine]);
        }
    }

    void FeeScene::doProcessEvent(gf::Event &event) {
        if (event.type == gf::EventType::MouseButtonPressed && event.mouseButton.button == gf::MouseButton::Left) {
            m_currentLine++;
            
            if (m_currentLine >= m_dialogueLines.size()) {
                m_game->popScene();
            } else {
                updateText();
            }
        }
    }

    void FeeScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states) {
        target.draw(m_box, states);
        target.draw(m_text, states);
        target.draw(m_clickHint, states);
    }
}