#include "DebutScene.h"
#include "RogueCMI.h"

namespace rCMI {

    static constexpr gf::Color4f PanelBg         = { 0.04f, 0.04f, 0.06f, 0.88f };
    static constexpr gf::Color4f TitleColor       = { 1.00f, 0.85f, 0.40f, 1.00f }; 
    static constexpr gf::Color4f TextColor        = { 0.85f, 0.80f, 0.70f, 1.00f }; 
    static constexpr gf::Color4f BorderColor      = { 0.55f, 0.42f, 0.18f, 1.00f };
    static constexpr gf::Color4f ButtonDefaultBg  = { 0.12f, 0.10f, 0.08f, 0.85f };
    static constexpr gf::Color4f ButtonDefaultTxt = { 0.85f, 0.80f, 0.70f, 1.00f };
    static constexpr gf::Color4f ButtonSelectedBg = { 0.55f, 0.38f, 0.10f, 0.95f };
    static constexpr gf::Color4f ButtonSelectedTxt= { 1.00f, 0.92f, 0.60f, 1.00f };

    DebutScene::DebutScene(RogueCMI *game)
    : gf::Scene(gf::Vector2f(1200, 800))
    , m_game(game)
    , m_titre("Le Donjon du CMI", game->resources.getFont(PATH_FONT), 50)
    , m_message("Votre quête commence ici : obtenir votre Licence !\n\nChaque niveau de ce donjon impitoyable represente un semestre.\nIl vous faudra survivre a 6 semestres afin d'esperer obtenir votre diplome.", game->resources.getFont(PATH_FONT), 26)
    , m_infoCommandes("Pour les commandes detaillees,\ncliquez sur l'icone des parametres en bas a droite.\n\nArmez-vous de courage et de perseverance,\ncar cette quete sera longue et ardue. Bonne chance !", game->resources.getFont(PATH_FONT), 22)
    , m_boutonOk("C'EST PARTI !", game->resources.getFont(PATH_FONT))
    {
        setClearColor(gf::Color::Transparent);

        m_titre.setColor(TitleColor);
        m_message.setColor(TextColor);
        m_infoCommandes.setColor(TextColor);

        m_boutonOk.setDefaultTextColor(ButtonDefaultTxt);
        m_boutonOk.setDefaultBackgroundColor(ButtonDefaultBg);
        m_boutonOk.setSelectedTextColor(ButtonSelectedTxt);
        m_boutonOk.setSelectedBackgroundColor(ButtonSelectedBg);
        m_boutonOk.setAnchor(gf::Anchor::Center);
        m_boutonOk.setCallback([&]() {
            m_game->m_WorldScene.m_isActivateDebut = false;
            m_game->popScene();
        });
        m_widgets.addWidget(m_boutonOk);
    }

    void DebutScene::doProcessEvent(gf::Event &event) {
        if (!isActive()) return;

        if (event.type == gf::EventType::MouseMoved)
            m_widgets.pointTo(event.mouseCursor.coords);

        if (event.type == gf::EventType::MouseButtonPressed && event.mouseButton.button == gf::MouseButton::Left)
            m_widgets.triggerAction();

        if (event.type == gf::EventType::KeyPressed &&
           (event.key.keycode == gf::Keycode::Return || event.key.keycode == gf::Keycode::Escape)) {
            m_game->m_WorldScene.m_isActivateDebut = false;
            m_game->popScene();
        }
    }

    void DebutScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states) {
        const gf::Vector2f vSize = target.getView().getSize();

        gf::Vector2f panelSize = { vSize.x * 0.52f, vSize.y * 0.82f };
        gf::Vector2f panelPos  = (vSize - panelSize) / 2.0f;
        float centerX = panelPos.x + panelSize.x / 2.0f;
        float textMaxWidth = panelSize.x * 0.85f;

        // --- Panneau de fond ---
        m_background.setSize(panelSize);
        m_background.setPosition(panelPos);
        m_background.setAnchor(gf::Anchor::TopLeft);
        m_background.setColor(PanelBg);
        m_background.setOutlineColor(BorderColor);
        m_background.setOutlineThickness(2.5f);
        target.draw(m_background, states);

        gf::RectangleShape separator;
        separator.setSize({ panelSize.x * 0.75f, 2.0f });
        separator.setColor(BorderColor);
        separator.setAnchor(gf::Anchor::Center);
        separator.setPosition({ centerX, panelPos.y + panelSize.y * 0.22f });
        target.draw(separator, states);

        m_titre.setAnchor(gf::Anchor::Center);
        m_titre.setPosition({ centerX, panelPos.y + panelSize.y * 0.13f });
        target.draw(m_titre, states);

        m_message.setParagraphWidth(textMaxWidth);
        m_message.setAlignment(gf::Alignment::Center);
        m_message.setAnchor(gf::Anchor::Center);
        m_message.setPosition({ centerX, panelPos.y + panelSize.y * 0.40f });
        target.draw(m_message, states);

        m_infoCommandes.setParagraphWidth(textMaxWidth);
        m_infoCommandes.setAlignment(gf::Alignment::Center);
        m_infoCommandes.setAnchor(gf::Anchor::Center);
        m_infoCommandes.setPosition({ centerX, panelPos.y + panelSize.y * 0.65f });
        target.draw(m_infoCommandes, states);

        gf::Vector2f boutonPos = { centerX, panelPos.y + panelSize.y * 0.87f };
        m_boutonOk.setAnchor(gf::Anchor::Center);
        m_boutonOk.setPosition(boutonPos);
        m_boutonOk.setParagraphWidth(textMaxWidth * 0.55f);
        m_boutonOk.setPadding(12.0f);
        m_widgets.render(target, states);
    }
}