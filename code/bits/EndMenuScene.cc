#include "EndMenuScene.h"
#include "RogueCMI.h"
#include <gf/Coordinates.h>

namespace rCMI
{
  static constexpr gf::Color4f PanelBg          = { 0.04f, 0.04f, 0.06f, 0.92f };
  static constexpr gf::Color4f TitleColor        = { 0.85f, 0.25f, 0.20f, 1.00f }; 
  static constexpr gf::Color4f TextColor         = { 0.85f, 0.80f, 0.70f, 1.00f }; 
  static constexpr gf::Color4f BorderColor       = { 0.50f, 0.15f, 0.12f, 1.00f }; 
  static constexpr gf::Color4f ButtonDefaultBg   = { 0.12f, 0.10f, 0.08f, 0.85f };
  static constexpr gf::Color4f ButtonDefaultTxt  = { 0.85f, 0.80f, 0.70f, 1.00f };
  static constexpr gf::Color4f ButtonSelectedBg  = { 0.85f, 0.25f, 0.20f, 1.00f };
  static constexpr gf::Color4f ButtonSelectedTxt = { 0.85f, 0.80f, 0.70f, 1.00f };
  static constexpr gf::Color4f ButtonDisabledBg  = { 0.85f, 0.25f, 0.20f, 1.00f };
  static constexpr gf::Color4f ButtonDisabledTxt = { 0.45f, 0.45f, 0.45f, 1.00f };

  EndMenuScene::EndMenuScene(RogueCMI *game)
      : gf::Scene(view_size),
        m_game(game),
        font(m_game->resources.getFont(PATH_FONT)),
        m_title("PARTIE TERMINÉE", font),
        m_statsText("", font),
        m_quit("Retourner au menu", font)
  {
    setClearColor(gf::Color::Black);

    m_title.setColor(TitleColor);
    m_title.setAnchor(gf::Anchor::Center);

    m_statsText.setColor(TextColor);
    m_statsText.setAnchor(gf::Anchor::Center);

    auto createButtons = [&](gf::TextButtonWidget &button, auto callback)
    {
      button.setDefaultTextColor(ButtonDefaultTxt);
      button.setDefaultBackgroundColor(ButtonDefaultBg);
      button.setSelectedTextColor(ButtonSelectedTxt);
      button.setSelectedBackgroundColor(ButtonSelectedBg);
      button.setDisabledTextColor(ButtonDisabledTxt);
      button.setDisabledBackgroundColor(ButtonDisabledBg);
      button.setAnchor(gf::Anchor::TopLeft);
      button.setAlignment(gf::Alignment::Center);
      button.setCallback(callback);
      widgets.addWidget(button);
    };

    createButtons(m_quit, [&]()
    {
      m_game->m_WorldScene.reset();
      m_game->popAllScenes();
      m_game->pushScene(m_game->m_MenuScene);
    });
  }

  void EndMenuScene::setFinalStats(int score, int kills, int highest_level)
  {
    std::string content  = "Score Final      :  " + std::to_string(score) + "\n";
    content             += "Ennemis vaincus  :  " + std::to_string(kills) + "\n";
    content             += "Niveau atteint   :  " + std::to_string(highest_level);
    m_statsText.setString(content);
  }

  void EndMenuScene::doProcessEvent(gf::Event &event)
  {
    if (event.type == gf::EventType::MouseMoved)
      widgets.pointTo(m_game->computeWindowToGameCoordinates(event.mouseCursor.coords, getHudView()));

    if (event.type == gf::EventType::MouseButtonPressed && event.mouseButton.button == gf::MouseButton::Left)
      widgets.triggerAction();
  }

  void EndMenuScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    target.setView(getHudView());
    const gf::Vector2f vSize = target.getView().getSize();
    gf::Coordinates coords(target);

    gf::Vector2f panelSize = { vSize.x * 0.55f, vSize.y * 0.72f };
    gf::Vector2f panelPos  = (vSize - panelSize) / 2.0f;
    float centerX = panelPos.x + panelSize.x / 2.0f;

    float size    = 0.04f;
    int   r_size  = coords.getRelativeCharacterSize(size);
    float padding = coords.getRelativeSize({0.01f, 0.f}).x;
    float btnW    = panelSize.x * 0.70f;

    m_background.setSize(panelSize);
    m_background.setPosition(panelPos);
    m_background.setColor(PanelBg);
    m_background.setOutlineColor(BorderColor);
    m_background.setOutlineThickness(2.5f);
    target.draw(m_background, states);

    gf::RectangleShape sep;
    sep.setSize({ panelSize.x * 0.75f, 2.0f });
    sep.setColor(BorderColor);
    sep.setAnchor(gf::Anchor::Center);
    sep.setPosition({ centerX, panelPos.y + panelSize.y * 0.22f });
    target.draw(sep, states);

    m_title.setCharacterSize(static_cast<unsigned int>(r_size * 1.4f));
    m_title.setAnchor(gf::Anchor::Center);
    m_title.setPosition({ centerX, panelPos.y + panelSize.y * 0.14f });
    target.draw(m_title, states);

    m_statsText.setCharacterSize(r_size);
    m_statsText.setParagraphWidth(panelSize.x * 0.85f);
    m_statsText.setAlignment(gf::Alignment::Left);
    m_statsText.setAnchor(gf::Anchor::Center);
    m_statsText.setPosition({ centerX, panelPos.y + panelSize.y * 0.50f });
    target.draw(m_statsText, states);

    m_quit.setCharacterSize(r_size);
    m_quit.setAnchor(gf::Anchor::Center);
    m_quit.setPosition({ centerX, panelPos.y + panelSize.y * 0.83f });
    m_quit.setParagraphWidth(btnW);
    m_quit.setPadding(padding);

    widgets.render(target, states);
  }
}