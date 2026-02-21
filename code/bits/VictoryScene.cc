#include "VictoryScene.h"
#include "RogueCMI.h"

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

    // Message principal
    m_messageText.setColor(gf::Color::Black);
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

    createButtons(m_continue, [&]() { 
      m_game->replaceScene(m_game->m_WorldScene);
      m_game->m_WorldScene.m_world_entity.nextLevel();
    });

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

  void VictoryScene::doProcessEvent(gf::Event &event)
  {
    if (event.type == gf::EventType::MouseMoved)
      widgets.pointTo(m_game->computeWindowToGameCoordinates(event.mouseCursor.coords, getHudView()));

    if (event.type == gf::EventType::MouseButtonPressed && event.mouseButton.button == gf::MouseButton::Left) {
      widgets.triggerAction();
    }
  }

  void VictoryScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    target.setView(getHudView());
    const gf::Vector2f vSize = target.getView().getSize();
    gf::Coordinates coords(target);

    gf::Vector2f invSize = { vSize.x * 0.45f, vSize.y * 0.80f };
    gf::Vector2f invPos = (vSize - invSize) / 2.0f;

    float size = 0.04f; 
    int r_size = coords.getRelativeCharacterSize(size);
    
    float textMaxWidth = invSize.x * 0.90f; 
    float buttonWidth = invSize.x * 0.7f; 
    float padding = coords.getRelativeSize({0.01f, 0.f}).x;

    float buttonY = invPos.y + invSize.y - (invSize.y * 0.25f);
    float continueY = invPos.y + invSize.y - (invSize.y * 0.35f);
    
    m_title.setCharacterSize(r_size * 1.5f);
    m_title.setAnchor(gf::Anchor::Center);
    m_title.setPosition({vSize.x / 2.0f, invPos.y + (invSize.y * 0.20f)});

    m_messageText.setCharacterSize(r_size * 0.7f);
    m_messageText.setParagraphWidth(textMaxWidth);
    m_messageText.setAlignment(gf::Alignment::Center); 
    m_messageText.setAnchor(gf::Anchor::Center);
    m_messageText.setPosition({vSize.x / 2.0f, invPos.y + (invSize.y * 0.40f)});

    m_scoreText.setCharacterSize(r_size * 1.2f);
    m_scoreText.setAnchor(gf::Anchor::Center);
    m_scoreText.setPosition({vSize.x / 2.0f, invPos.y + (invSize.y * 0.65f)});

    m_continue.setCharacterSize(r_size);
    m_continue.setAnchor(gf::Anchor::Center);
    m_continue.setPosition({ vSize.x / 2.0f, continueY });
    m_continue.setParagraphWidth(buttonWidth);
    m_continue.setPadding(padding);

    m_quit.setCharacterSize(r_size);
    m_quit.setAnchor(gf::Anchor::Center);
    m_quit.setPosition({ vSize.x / 2.0f, buttonY });
    m_quit.setParagraphWidth(buttonWidth);
    m_quit.setPadding(padding);

    m_background.setSize(invSize);
    m_background.setPosition(invPos);
    target.draw(m_background, states);
    
    target.draw(m_title, states);
    target.draw(m_messageText, states);
    target.draw(m_scoreText, states);
    widgets.render(target, states);
  }
}