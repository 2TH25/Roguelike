#include "DebutScene.h"
#include "RogueCMI.h"

namespace rCMI {

    DebutScene::DebutScene(RogueCMI *game)
    : gf::Scene(gf::Vector2f(1200, 800))
    , m_game(game)
    , m_titre("BIENVENUE", game->resources.getFont(PATH_FONT), 60)
    , m_message("Votre aventure dans le donjon commence !", game->resources.getFont(PATH_FONT), 30)
    , m_infoCommandes("Pour les commandes detaillées,\ncliquez sur l'icone des paramètres, qui se trouve en bas à droite.\n Et bien voilà. Il ne me reste qu'à vous souhaiter beaucoup de courage, et un bon jeu !", game->resources.getFont(PATH_FONT), 22)
    , m_boutonOk("OK", game->resources.getFont(PATH_FONT))
    {
        setClearColor(gf::Color::Transparent);

        m_background.setSize({750.0f, 500.0f});
        m_background.setAnchor(gf::Anchor::Center);
        m_background.setPosition({600.0f, 400.0f});
        m_background.setOutlineColor(gf::Color::White);
        m_background.setOutlineThickness(2.0f);
        m_background.setTexture(game->resources.getTexture("backgroundItem.png"));

        m_titre.setAnchor(gf::Anchor::Center);
        m_titre.setPosition({600.0f, 250.0f});
        m_titre.setColor(gf::Color::Yellow);

        m_message.setAnchor(gf::Anchor::Center);
        m_message.setPosition({600.0f, 350.0f});

        m_infoCommandes.setAnchor(gf::Anchor::Center);
        m_infoCommandes.setPosition({600.0f, 450.0f});
        m_infoCommandes.setAlignment(gf::Alignment::Center);

        
        m_boutonOk.setSelected(); 
        m_boutonOk.setAnchor(gf::Anchor::Center);
        m_boutonOk.setPosition({600.0f, 580.0f});
    }

    void DebutScene::doProcessEvent(gf::Event &event) {
        if (!isActive()) {
            return;
        }

        if (event.type == gf::EventType::MouseButtonPressed && event.mouseButton.button == gf::MouseButton::Left) {
            gf::Vector2f mouseCoords = event.mouseButton.coords;

            if (!m_boutonOk.isDisabled() && m_boutonOk.contains(mouseCoords)) {
                m_game->popScene();
                return;
            }
        }
        
        if (event.type == gf::EventType::KeyPressed && 
           (event.key.keycode == gf::Keycode::Return || event.key.keycode == gf::Keycode::Escape)) {
            m_game->popScene();
        }
    }

    void DebutScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states) {
        const gf::Vector2f vSize = target.getView().getSize();
        
        gf::Vector2f panelSize = { vSize.x * 0.50f, vSize.y * 0.80f };
        gf::Vector2f panelPos = (vSize - panelSize) / 2.0f;

        m_background.setSize(panelSize);
        m_background.setPosition(panelPos);
        m_background.setAnchor(gf::Anchor::TopLeft); 
        target.draw(m_background, states);

        float centerX = panelPos.x + (panelSize.x / 2.0f);    
        float textMaxWidth = panelSize.x * 0.90f;

        // --- Textes (restent centrés) ---
        m_titre.setAnchor(gf::Anchor::Center);
        m_titre.setPosition({centerX, panelPos.y + (panelSize.y * 0.15f)});
        
        m_message.setParagraphWidth(textMaxWidth); 
        m_message.setAlignment(gf::Alignment::Center);
        m_message.setAnchor(gf::Anchor::Center);
        m_message.setPosition({centerX, panelPos.y + (panelSize.y * 0.35f)});
        
        m_infoCommandes.setParagraphWidth(textMaxWidth);
        m_infoCommandes.setAlignment(gf::Alignment::Center);
        m_infoCommandes.setAnchor(gf::Anchor::Center);
        m_infoCommandes.setPosition({centerX, panelPos.y + (panelSize.y * 0.60f)});
        

        float margin = 100.0f;
        gf::Vector2f boutonPos = { 
            panelPos.x + panelSize.x - margin, 
            panelPos.y + panelSize.y - margin 
        };

        gf::RectF textBounds = m_boutonOk.getLocalBounds();
        float textWidth = textBounds.max.x - textBounds.min.x;
        float textHeight = textBounds.max.y - textBounds.min.y;


        gf::Vector2f bgSize = { textWidth + 40.0f, textHeight + 20.0f };
        m_boutonBackground.setSize(bgSize);
        m_boutonBackground.setAnchor(gf::Anchor::Center);
        m_boutonBackground.setPosition(boutonPos);

        m_boutonOk.setAnchor(gf::Anchor::Center);
        m_boutonOk.setPosition(boutonPos);
        m_boutonOk.setDefaultTextColor(gf::Color::Black);

        m_boutonBackground.setColor(gf::Color::White); 
        m_boutonBackground.setOutlineColor(gf::Color::White);
        m_boutonBackground.setOutlineThickness(2.0f);

        if (!m_boutonOk.isDisabled()) {
            target.draw(m_boutonBackground, states);
            m_boutonOk.draw(target, states);
        }

        target.draw(m_titre, states);
        target.draw(m_message, states);
        target.draw(m_infoCommandes, states);
        
    }
}