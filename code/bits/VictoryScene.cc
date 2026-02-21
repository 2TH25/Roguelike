#include "VictoryScene.h"
#include "RogueCMI.h"

namespace rCMI {

namespace rCMI
{
  VictoryScene::VictoryScene(RogueCMI *game)
      : gf::Scene(view_size),
        m_game(game),
        font(m_game->resources.getFont(PATH_FONT)),
        m_title("FÉLICITATIONS !", font),
        m_messageText("Bravo ! Tu as atteint la fin du donjon !\nTu as reussi a finir ces 6 semestres\net a obtenir ton diplome de licence !\n\nGrand bravo a toi ! Voici ton score :", font),
        m_scoreText("", font),
        m_quit("Retourner au menu", font),
        m_continue("Continuer l'aventure", font)
  {
    setClearColor(gf::Color::Black);
    
    // Titre
    m_title.setColor(gf::Color::Yellow);
    m_title.setAnchor(gf::Anchor::Center);

        m_messageText.setAnchor(gf::Anchor::Center);
        m_messageText.setAlignment(gf::Alignment::Center);

        m_scoreText.setColor(gf::Color::Green);
        m_scoreText.setAnchor(gf::Anchor::Center);

    m_background.setTexture(game->resources.getTexture("BackgroundInventory.png"));

    auto createButtons = [&](gf::TextButtonWidget &button, auto callback)
    {
      button.setDefaultTextColor(gf::Color::Black);
      button.setDefaultBackgroundColor(gf::Color::Gray(0.7f));
      button.setSelectedTextColor(gf::Color::Black);
      button.setSelectedBackgroundColor(gf::Color::Green);
      button.setDisabledTextColor(gf::Color::Black);
      button.setDisabledBackgroundColor(gf::Color::Red);
      button.setAnchor(gf::Anchor::TopLeft);
      button.setAlignment(gf::Alignment::Center);
      button.setCallback(callback);
      widgets.addWidget(button);
    };

    createButtons(m_quit, [&]()
    { 
      m_game->popAllScenes(); 
      m_game->pushScene(m_game->m_MenuScene);
    });
  }

  void VictoryScene::setFinalScore(int score) 
  {
    m_scoreText.setString(std::to_string(score) + " POINTS");
  }

    void VictoryScene::doProcessEvent(gf::Event &event) {
        if (!isActive()) return;

    if (event.type == gf::EventType::MouseButtonPressed && event.mouseButton.button == gf::MouseButton::Left) {
      widgets.triggerAction();
    }
  }

  void VictoryScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    target.setView(getHudView());
    const gf::Vector2f vSize = target.getView().getSize();

    gf::Vector2f invSize = { vSize.x * 0.45f, vSize.y * 0.90f };
    gf::Vector2f invPos = (vSize - invSize) / 2.0f;

    float size = 0.04f; 
    int r_size = coords.getRelativeCharacterSize(size);
    float buttonWidth = invSize.x * 0.8f; 
    float padding = coords.getRelativeSize({0.01f, 0.f}).x;

    float buttonY = invPos.y + invSize.y - (invSize.y * 0.25f);
    
    // Bouton 
    m_quit.setCharacterSize(r_size);
    m_quit.setAnchor(gf::Anchor::Center);
    m_quit.setPosition({ vSize.x / 2.0f, buttonY });
    m_quit.setParagraphWidth(buttonWidth);
    m_quit.setPadding(padding);

    // Textes
    m_title.setCharacterSize(r_size * 1.5f);
    m_title.setPosition(coords.getRelativePoint({0.5f, 0.15f}));

    m_messageText.setCharacterSize(r_size * 0.8f);
    m_messageText.setPosition(coords.getRelativePoint({0.5f, 0.40f}));

    m_scoreText.setCharacterSize(r_size * 1.5f);
    m_scoreText.setPosition(coords.getRelativePoint({0.5f, 0.60f}));

    // Dessin
    m_background.setSize(invSize);
    m_background.setPosition(invPos);
    m_background.draw(target, states);
    
    target.draw(m_title, states);
    target.draw(m_messageText, states);
    target.draw(m_scoreText, states);
    widgets.render(target, states);
  }
}
}