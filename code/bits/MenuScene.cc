#include "MenuScene.h"

#include "RogueCMI.h"

#include <gf/Coordinates.h>

namespace rCMI
{
  MenuScene::MenuScene(RogueCMI *game)
      : gf::Scene(view_size),
        m_game(game), up("up"),
        down("down"), select("select"),
        font(m_game->resources.getFont(PATH_FONT)),
        m_saves("jouer", font),
        m_quit("quitter", font),
        m_test("test zone", font)
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

    createButtons(m_saves, [&]()
                  { m_game->m_WorldScene.reset();
                    m_game->replaceScene(m_game->m_WorldScene);
                    m_game->pushScene(m_game->m_DebutScene);
                  });

    createButtons(m_quit, [&]()
                  { m_game->popAllScenes(); });

    createButtons(m_test, [&]()
                  { m_game->m_WorldScene.generateMap(TestMapSize);
                    m_game->replaceScene(m_game->m_WorldScene);
                    m_game->pushScene(m_game->m_DebutScene);
                  });
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

  // TODO: marche mais à changer
  void MenuScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    float size = 0.05f, space = 0.2f;
    gf::Vector2f bg_size(0.55f, 0.4f);
    target.setView(getHudView());
    gf::Coordinates coords(target);
    float width = coords.getRelativeSize(bg_size - 0.05f).x, padding = coords.getRelativeSize({0.01f, 0.f}).x;
    int r_size = coords.getRelativeCharacterSize(size);

    m_saves.setCharacterSize(r_size);
    m_saves.setAnchor(gf::Anchor::Center);
    m_saves.setPosition(coords.getRelativePoint({0.5f, 0.425f}));
    m_saves.setParagraphWidth(width);
    m_saves.setPadding(padding);

    m_quit.setCharacterSize(r_size);
    m_quit.setAnchor(gf::Anchor::Center);
    m_quit.setPosition(coords.getRelativePoint({0.5f, 0.525f}));
    m_quit.setParagraphWidth(width);
    m_quit.setPadding(padding);

    m_test.setCharacterSize(r_size);
    m_test.setAnchor(gf::Anchor::Center);
    m_test.setPosition(coords.getRelativePoint({0.5f, 0.625f}));
    m_test.setParagraphWidth(width);
    m_test.setPadding(padding);

    widgets.render(target, states);
  }
}