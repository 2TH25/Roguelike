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

    m_statsText.setColor(gf::Color::White);
    m_statsText.setAnchor(gf::Anchor::Center);


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
    content += "Ennemis vaincus : " + std::to_string(kills);
    content += "Niveau atteint : " + std::to_string(highest_level);
    
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
    float size = 0.05f, space = 0.2f;
    gf::Vector2f bg_size(0.55f, 0.4f);
    target.setView(getHudView());
    gf::Coordinates coords(target);
    float width = coords.getRelativeSize(bg_size - 0.05f).x, padding = coords.getRelativeSize({0.01f, 0.f}).x;
    int r_size = coords.getRelativeCharacterSize(size);

    m_quit.setCharacterSize(r_size);
    m_quit.setAnchor(gf::Anchor::Center);
    m_quit.setPosition(coords.getRelativePoint({0.5f, 0.525f}));
    m_quit.setParagraphWidth(width);
    m_quit.setPadding(padding);

    widgets.render(target, states);
  }
}