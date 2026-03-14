#include "VictoryScene.h"
#include "RogueCMI.h"

namespace rCMI
{
  static constexpr gf::Color4f PanelBg          =  { 0.04f, 0.06f, 0.04f, 0.92f }; 
  static constexpr gf::Color4f TitleColor        = { 0.25f, 0.50f, 0.20f, 1.00f }; 
  static constexpr gf::Color4f TextColor         = { 0.85f, 0.80f, 0.70f, 1.00f }; 
  static constexpr gf::Color4f ScoreColor        = { 0.25f, 0.50f, 0.20f, 1.00f }; 
  static constexpr gf::Color4f BorderColor       = { 0.25f, 0.50f, 0.20f, 1.00f };
  static constexpr gf::Color4f ButtonDefaultBg   = { 0.12f, 0.10f, 0.08f, 0.85f };
  static constexpr gf::Color4f ButtonDefaultTxt  = { 0.85f, 0.80f, 0.70f, 1.00f };
  static constexpr gf::Color4f ButtonSelectedBg  = { 0.25f, 0.50f, 0.20f, 1.00f };
  static constexpr gf::Color4f ButtonSelectedTxt = { 1.00f, 0.92f, 0.60f, 1.00f };
  static constexpr gf::Color4f ButtonDisabledBg  = { 0.20f, 0.20f, 0.20f, 0.60f };
  static constexpr gf::Color4f ButtonDisabledTxt = { 0.45f, 0.45f, 0.45f, 1.00f };

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

    m_title.setColor(TitleColor);
    m_title.setAnchor(gf::Anchor::Center);

    m_messageText.setColor(TextColor);
    m_messageText.setAnchor(gf::Anchor::Center);
    m_messageText.setAlignment(gf::Alignment::Center);

    m_scoreText.setColor(ScoreColor);
    m_scoreText.setAnchor(gf::Anchor::Center);

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

    createButtons(m_continue, [&]() {
      m_game->m_WorldScene.requestInputLock();
      m_game->replaceScene(m_game->m_WorldScene);
      m_game->m_WorldScene.m_world_entity.nextLevel();
    });

    createButtons(m_quit, [&]()
    {
      m_game->m_InventoryScene->m_inventory.reset(m_game);
      m_game->popAllScenes();
      m_game->pushScene(m_game->m_MenuScene);
    });
  }

  void VictoryScene::setFinalScore(int score)
  {
    m_scoreText.setString(std::to_string(score) + "  POINTS");
  }

  void VictoryScene::doProcessEvent(gf::Event &event)
  {
    if (event.type == gf::EventType::MouseMoved)
      widgets.pointTo(m_game->computeWindowToGameCoordinates(event.mouseCursor.coords, getHudView()));

    if (event.type == gf::EventType::MouseButtonPressed && event.mouseButton.button == gf::MouseButton::Left)
      widgets.triggerAction();
  }

  void VictoryScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    target.setView(getHudView());
    const gf::Vector2f vSize = target.getView().getSize();
    gf::Coordinates coords(target);

    gf::Vector2f panelSize = { vSize.x * 0.46f, vSize.y * 0.80f };
    gf::Vector2f panelPos  = (vSize - panelSize) / 2.0f;
    float centerX    = panelPos.x + panelSize.x / 2.0f;
    float textMaxW   = panelSize.x * 0.85f;
    float btnW       = panelSize.x * 0.65f;

    float size   = 0.04f;
    int   r_size = coords.getRelativeCharacterSize(size);
    float padding = coords.getRelativeSize({0.01f, 0.f}).x;

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

    m_title.setCharacterSize(static_cast<unsigned int>(r_size * 1.5f));
    m_title.setAnchor(gf::Anchor::Center);
    m_title.setPosition({ centerX, panelPos.y + panelSize.y * 0.13f });
    target.draw(m_title, states);

    m_messageText.setCharacterSize(static_cast<unsigned int>(r_size * 0.75f));
    m_messageText.setParagraphWidth(textMaxW);
    m_messageText.setAlignment(gf::Alignment::Center);
    m_messageText.setAnchor(gf::Anchor::Center);
    m_messageText.setPosition({ centerX, panelPos.y + panelSize.y * 0.42f });
    target.draw(m_messageText, states);

    m_scoreText.setCharacterSize(static_cast<unsigned int>(r_size * 1.3f));
    m_scoreText.setAnchor(gf::Anchor::Center);
    m_scoreText.setPosition({ centerX, panelPos.y + panelSize.y * 0.64f });
    target.draw(m_scoreText, states);

    m_continue.setCharacterSize(r_size);
    m_continue.setAnchor(gf::Anchor::Center);
    m_continue.setPosition({ centerX, panelPos.y + panelSize.y * 0.77f });
    m_continue.setParagraphWidth(btnW);
    m_continue.setPadding(padding);

    m_quit.setCharacterSize(r_size);
    m_quit.setAnchor(gf::Anchor::Center);
    m_quit.setPosition({ centerX, panelPos.y + panelSize.y * 0.89f });
    m_quit.setParagraphWidth(btnW);
    m_quit.setPadding(padding);

    widgets.render(target, states);
  }
}