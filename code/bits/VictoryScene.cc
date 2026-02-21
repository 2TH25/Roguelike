#include "VictoryScene.h"
#include "RogueCMI.h"

namespace rCMI {

    VictoryScene::VictoryScene(RogueCMI *game)
    : gf::Scene(gf::Vector2f(1200, 800))
    , m_game(game)
    , m_title("FÉLICITATIONS !", game->resources.getFont(PATH_FONT), 60)
    , m_messageText("Bravo ! Tu as atteint la fin du donjon !\nTu as réussi à finir ces 6 semestres\net à obtenir ton diplôme de licence !\n\nGrand bravo à toi !", game->resources.getFont(PATH_FONT), 25)
    , m_scoreText("0 POINTS", game->resources.getFont(PATH_FONT), 40)
    , m_boutonQuit("RETOUR AU MENU", game->resources.getFont(PATH_FONT))
    {
        setClearColor(gf::Color::Transparent);

        m_background.setOutlineColor(gf::Color::White);
        m_background.setOutlineThickness(2.0f);
        m_background.setTexture(game->resources.getTexture("BackgroundInventory.png"));

        m_title.setColor(gf::Color::Yellow);
        m_title.setAnchor(gf::Anchor::Center);

        m_messageText.setAnchor(gf::Anchor::Center);
        m_messageText.setAlignment(gf::Alignment::Center);

        m_scoreText.setColor(gf::Color::Green);
        m_scoreText.setAnchor(gf::Anchor::Center);

        m_boutonQuit.setSelected(); 
        m_boutonQuit.setAnchor(gf::Anchor::Center);
    }

    void VictoryScene::setFinalScore(int score) {
        m_scoreText.setString(std::to_string(score) + " POINTS");
    }

    void VictoryScene::doProcessEvent(gf::Event &event) {
        if (!isActive()) return;

        if (event.type == gf::EventType::MouseButtonPressed && event.mouseButton.button == gf::MouseButton::Left) {
            gf::Vector2f mouseCoords = event.mouseButton.coords;
            if (m_boutonQuit.contains(mouseCoords)) {
                m_game->popAllScenes();
                m_game->pushScene(m_game->m_MenuScene);
            }
        }
        
        if (event.type == gf::EventType::KeyPressed && 
           (event.key.keycode == gf::Keycode::Return || event.key.keycode == gf::Keycode::Escape)) {
            m_game->popAllScenes();
            m_game->pushScene(m_game->m_MenuScene);
        }
    }

    void VictoryScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states) {
        const gf::Vector2f vSize = target.getView().getSize();
        
        gf::Vector2f panelSize = { vSize.x * 0.60f, vSize.y * 0.85f };
        gf::Vector2f panelPos = (vSize - panelSize) / 2.0f;

        m_background.setSize(panelSize);
        m_background.setPosition(panelPos);
        m_background.setAnchor(gf::Anchor::TopLeft); 
        target.draw(m_background, states);

        float centerX = panelPos.x + (panelSize.x / 2.0f);    
        float textMaxWidth = panelSize.x * 0.90f;

        m_title.setPosition({centerX, panelPos.y + (panelSize.y * 0.15f)});
        
        m_messageText.setParagraphWidth(textMaxWidth); 
        m_messageText.setPosition({centerX, panelPos.y + (panelSize.y * 0.40f)});
        
        m_scoreText.setPosition({centerX, panelPos.y + (panelSize.y * 0.65f)});

        float margin = 80.0f;
        gf::Vector2f boutonPos = { centerX, panelPos.y + panelSize.y - margin };

        gf::RectF textBounds = m_boutonQuit.getLocalBounds();
        float textWidth = textBounds.max.x - textBounds.min.x;
        float textHeight = textBounds.max.y - textBounds.min.y;

        gf::Vector2f bgSize = { textWidth + 60.0f, textHeight + 20.0f };
        m_boutonBackground.setSize(bgSize);
        m_boutonBackground.setAnchor(gf::Anchor::Center);
        m_boutonBackground.setPosition(boutonPos);
        m_boutonBackground.setColor(gf::Color::White); 

        m_boutonQuit.setPosition(boutonPos);
        m_boutonQuit.setDefaultTextColor(gf::Color::Black);

        target.draw(m_boutonBackground, states);
        m_boutonQuit.draw(target, states);
        target.draw(m_title, states);
        target.draw(m_messageText, states);
        target.draw(m_scoreText, states);
    }
}