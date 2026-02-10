#include "EndMenuScene.h"
#include "RogueCMI.h"
#include <gf/Coordinates.h>

namespace rCMI
{
  EndMenuScene::EndMenuScene(RogueCMI *game)
      : gf::Scene(view_size),
        m_game(game),
        font(m_game->resources.getFont(PATH_FONT)),
        m_title("PARTIE TERMINÉE", font),
        m_statsText("", font),
        m_quit("Retourner au menu", font)
  {
    setClearColor(gf::Color::Black);
    m_title.setColor(gf::Color::Yellow);
    m_title.setAnchor(gf::Anchor::Center);

    m_statsText.setColor(gf::Color::Black);
    m_statsText.setAnchor(gf::Anchor::Center);

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

  void EndMenuScene::setFinalStats(int score, int kills, int highest_level) 
  {
    std::string content = "Score Final : " + std::to_string(score) + "\n";
    content += "Ennemis vaincus : " + std::to_string(kills) + "\n";
    content += "Niveau atteint : " + std::to_string(highest_level) + "\n";
    
    m_statsText.setString(content);
  }


  void EndMenuScene::doProcessEvent(gf::Event &event)
  {
    if (event.type == gf::EventType::MouseMoved)
      widgets.pointTo(m_game->computeWindowToGameCoordinates(event.mouseCursor.coords, getHudView()));

    if (event.type == gf::EventType::MouseButtonPressed && event.mouseButton.button == gf::MouseButton::Left) {
      widgets.triggerAction();
  }
  }

  void EndMenuScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states)
{
    target.setView(getHudView());
    const gf::Vector2f vSize = target.getView().getSize();
    gf::Coordinates coords(target);

    gf::Vector2f invSize = { vSize.x * 0.45f, vSize.y * 0.90f };
    gf::Vector2f invPos = (vSize - invSize) / 2.0f;

    float size = 0.04f; 
    int r_size = coords.getRelativeCharacterSize(size);
    float buttonWidth = invSize.x * 0.8f; 
    float padding = coords.getRelativeSize({0.01f, 0.f}).x;

    float buttonY = invPos.y + invSize.y - (invSize.y * 0.25f);
    
    m_quit.setCharacterSize(r_size);
    m_quit.setAnchor(gf::Anchor::Center);
    m_quit.setPosition({ vSize.x / 2.0f, buttonY });
    m_quit.setParagraphWidth(buttonWidth);
    m_quit.setPadding(padding);

    m_statsText.setCharacterSize(r_size);
    m_statsText.setAnchor(gf::Anchor::Center); 
    m_statsText.setPosition(coords.getRelativePoint({0.5f, 0.35f}));

    m_background.setSize(invSize);
    m_background.setPosition(invPos);
    m_background.draw(target, states);
    
    widgets.render(target, states);
    target.draw(m_statsText, states);
}
}