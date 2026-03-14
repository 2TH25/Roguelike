#include "MenuScene.h"

#include "RogueCMI.h"

#include <gf/Coordinates.h>

namespace rCMI
{
  static constexpr gf::Color4f ButtonDefaultBg    = { 0.05f, 0.05f, 0.05f, 0.75f };
  static constexpr gf::Color4f ButtonDefaultText  = { 0.85f, 0.80f, 0.70f, 1.00f };
  static constexpr gf::Color4f ButtonSelectedBg   = { 0.55f, 0.38f, 0.10f, 0.92f };
  static constexpr gf::Color4f ButtonSelectedText = { 1.00f, 0.92f, 0.60f, 1.00f };
  static constexpr gf::Color4f ButtonDisabledBg   = { 0.20f, 0.20f, 0.20f, 0.60f }; 
  static constexpr gf::Color4f ButtonDisabledText = { 0.45f, 0.45f, 0.45f, 1.00f }; 

  MenuScene::MenuScene(RogueCMI *game)
      : gf::Scene(view_size),
        m_game(game), up("up"),
        down("down"), select("select"),
        font(m_game->resources.getFont(PATH_FONT)),
        m_saves("JOUER", font),
        m_quit("QUITTER", font),
        m_backgroundTexture(m_game->resources.getTexture("Menu.png")),
        m_background(m_backgroundTexture)
  {
    setClearColor(gf::Color::Black);

    up.addKeycodeKeyControl(gf::Keycode::Up);
    addAction(up);

    down.addKeycodeKeyControl(gf::Keycode::Down);
    addAction(down);

    select.addKeycodeKeyControl(gf::Keycode::Return);
    select.addMouseButtonControl(gf::MouseButton::Left);
    addAction(select);

    auto createButtons = [&](gf::TextButtonWidget &button, auto callback)
    {
      button.setDefaultTextColor(ButtonDefaultText);
      button.setDefaultBackgroundColor(ButtonDefaultBg);
      button.setSelectedTextColor(ButtonSelectedText);
      button.setSelectedBackgroundColor(ButtonSelectedBg);
      button.setDisabledTextColor(ButtonDisabledText);
      button.setDisabledBackgroundColor(ButtonDisabledBg);
      button.setAnchor(gf::Anchor::TopLeft);
      button.setAlignment(gf::Alignment::Center);
      button.setCallback(callback);
      widgets.addWidget(button);
    };

    createButtons(m_saves, [&]()
                  { m_game->m_WorldScene.reset();
                    m_game->m_InventoryScene->m_inventory.reset(m_game);
                    m_game->replaceScene(m_game->m_WorldScene);
                    m_game->m_WorldScene.m_isActivateDebut = true;
                    m_game->pushScene(m_game->m_DebutScene);
                  });

    createButtons(m_quit, [&]()
                  { m_game->popAllScenes(); });
  }

  void MenuScene::doHandleActions([[maybe_unused]] gf::Window &window)
  {
    if (up.isActive())
      widgets.selectPreviousWidget();

    if (down.isActive())
      widgets.selectNextWidget();

    if (select.isActive())
      widgets.triggerAction();
  }

  void MenuScene::doProcessEvent(gf::Event &event)
  {
    if (event.type == gf::EventType::MouseMoved)
      widgets.pointTo(m_game->computeWindowToGameCoordinates(event.mouseCursor.coords, getHudView()));
  }

  void MenuScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    target.setView(getHudView());
    gf::Coordinates coords(target);

    gf::Vector2f viewSize = getHudView().getSize();
    gf::Vector2u texSize  = m_backgroundTexture.getSize();

    // On scale l'image pour couvrir toute la vue
    float scaleX = viewSize.x / static_cast<float>(texSize.x);
    float scaleY = viewSize.y / static_cast<float>(texSize.y);
    float scale  = std::max(scaleX, scaleY);

    m_background.setScale(scale);
    m_background.setAnchor(gf::Anchor::Center);
    m_background.setPosition(getHudView().getCenter());
    target.draw(m_background, states);

    float size = 0.05f;
    gf::Vector2f bg_size(0.55f, 0.4f);
    float width   = coords.getRelativeSize(bg_size - 0.05f).x;
    float padding = coords.getRelativeSize({0.01f, 0.f}).x;
    int   r_size  = coords.getRelativeCharacterSize(size);
 
    m_saves.setCharacterSize(r_size);
    m_saves.setAnchor(gf::Anchor::Center);
    m_saves.setPosition(coords.getRelativePoint({0.5f, 0.60f}));
    m_saves.setParagraphWidth(width);
    m_saves.setPadding(padding);
 
    m_quit.setCharacterSize(r_size);
    m_quit.setAnchor(gf::Anchor::Center);
    m_quit.setPosition(coords.getRelativePoint({0.5f, 0.70f}));
    m_quit.setParagraphWidth(width);
    m_quit.setPadding(padding);
 
    widgets.render(target, states);
  }
}