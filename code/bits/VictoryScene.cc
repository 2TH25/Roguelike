#include "VictoryScene.h"
#include "RogueCMI.h"

namespace rCMI
{
  VictoryScene::VictoryScene(RogueCMI *game)
      : gf::Scene(view_size),
        m_game(game),
        font(m_game->resources.getFont(PATH_FONT)),
        m_title("FELICITATIONS, DIPLOME(E) !", font),
        m_messageText("Incroyable ! Tu as survecu aux 6 semestres de ce donjon infernal !\n\nMalgre l'extreme difficulte de la quete, tu as triomphe\ndes partiels et des monstres pour decrocher ta Licence !", font),
        m_statsText("", font),
        m_quit("Quitter la fac", font)
  {
    setClearColor(gf::Color::Black);
    
    m_title.setColor(gf::Color::Yellow);
    m_title.setAnchor(gf::Anchor::Center);

    m_messageText.setColor(gf::Color::Black);
    m_messageText.setAnchor(gf::Anchor::Center);
    m_messageText.setAlignment(gf::Alignment::Center);

    m_statsText.setColor(gf::Color::Blue);
    m_statsText.setAnchor(gf::Anchor::Center);
    m_statsText.setAlignment(gf::Alignment::Center);

    m_background.setTexture(game->resources.getTexture("backgroundItem.png"));

    auto createButtons = [&](gf::TextButtonWidget &button, auto callback)
    {
      button.setDefaultTextColor(gf::Color::Black);
      button.setDefaultBackgroundColor(gf::Color::Gray(0.7f));
      button.setSelectedTextColor(gf::Color::Black);
      button.setSelectedBackgroundColor(gf::Color::Green);
      button.setAnchor(gf::Anchor::Center); // Centrage du bouton
      button.setCallback(callback);
      widgets.addWidget(button);
    };

    createButtons(m_quit, [&]()
    { 
      m_game->m_InventoryScene->m_inventory.reset(m_game);
      m_game->popAllScenes(); 
      m_game->pushScene(m_game->m_MenuScene);
    });
  }

  void VictoryScene::setFinalStats(int score, int kills, int highest_level) 
  {
    std::string content = "Moyenne Finale (Score) : " + std::to_string(score) + "\n";
    content += "UE Validees (Ennemis) : " + std::to_string(kills) + "\n";
    content += "Semestre atteint : " + std::to_string(highest_level);
    
    m_statsText.setString(content);
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

    gf::Vector2f invSize = { vSize.x * 0.50f, vSize.y * 0.85f };
    gf::Vector2f invPos = (vSize - invSize) / 2.0f;
    float centerX = vSize.x / 2.0f; 

    float size = 0.04f; 
    int r_size = coords.getRelativeCharacterSize(size);

    m_title.setCharacterSize(r_size * 1.3f);
    m_title.setAnchor(gf::Anchor::Center); 
    m_title.setPosition({centerX, invPos.y + (invSize.y * 0.20f)});

    float messageMaxWidth = invSize.x * 0.65f; 
    m_messageText.setCharacterSize(r_size * 0.9f);
    m_messageText.setParagraphWidth(messageMaxWidth);
    m_messageText.setAnchor(gf::Anchor::Center);
    m_messageText.setPosition({centerX, invPos.y + (invSize.y * 0.45f)});

    m_statsText.setCharacterSize(r_size * 0.8f);
    m_statsText.setAnchor(gf::Anchor::Center);
    m_statsText.setPosition({centerX, invPos.y + (invSize.y * 0.68f)});

    float buttonY = invPos.y + invSize.y - (invSize.y * 0.18f);
    m_quit.setCharacterSize(r_size);
    m_quit.setAnchor(gf::Anchor::Center);
    m_quit.setPosition({ centerX, buttonY }); 
    m_quit.setParagraphWidth(invSize.x * 0.6f);

    // Affichage
    m_background.setSize(invSize);
    m_background.setPosition(invPos);
    target.draw(m_background, states);
    
    target.draw(m_title, states);
    target.draw(m_messageText, states);
    target.draw(m_statsText, states);
    widgets.render(target, states);
  }
}