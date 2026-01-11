#include "WorldScene.h"

#include "RogueCMI.h"
#include <iostream>

namespace rCMI
{
  WorldScene::WorldScene(RogueCMI *game)
  : gf::Scene(view_size)
  , m_game(game)
  , m_map(game)
  , m_actions(getActions())
  {
    setClearColor(gf::Color::Black);
    setWorldViewSize(view_size);
    gf::Vector2i TileVect({TileSize, TileSize});
    setWorldViewCenter(m_map.hero().getExistence().getPosition() * 80 + TileVect / 2);

    for (gf::Action *action : m_actions)
      addAction(*action);

    addWorldEntity(m_map);
    // addWorldEntity(m_hero);
    // addWorldEntity(m_slime);
    // addWorldEntity(m_skeleton);
    // addWorldEntity(m_zombie);

    const gf::Texture &textureMort = m_game->resources.getTexture("mort.png");

    for (auto &character : m_map.getCharacters())
      character.setDeadTexture(textureMort);
  }

  void WorldScene::doHandleActions(gf::Window &window)
  {
    Character &heroInMap = m_map.hero();

    bool playerMoved = false;

    if (Controls::isActiveAction("move_up", m_actions))
    {
      heroInMap.goUp(m_map);
      playerMoved = true;
    }
    else if (Controls::isActiveAction("move_down", m_actions))
    {
      heroInMap.goDown(m_map);
      playerMoved = true;
    }
    else if (Controls::isActiveAction("move_right", m_actions))
    {
      heroInMap.goRight(m_map);
      playerMoved = true;
    }
    else if (Controls::isActiveAction("move_left", m_actions))
    {
      heroInMap.goLeft(m_map);
      playerMoved = true;
    }

    if (Controls::isActiveAction("zoom_cam", m_actions))
    {
      setWorldViewSize(getWorldView().getSize() / 1.5);
    }

    if (Controls::isActiveAction("unzoom_cam", m_actions))
    {
      setWorldViewSize(getWorldView().getSize() * 1.5);
    }

    if (playerMoved)
    {
      m_map.EnemyTurns();
    }
  }

  void WorldScene::doUpdate([[maybe_unused]] gf::Time time)
  {
    gf::Vector2i TileVect({TileSize, TileSize});
    setWorldViewCenter(m_map.hero().getExistence().getPosition() * 80 + TileVect / 2);
    // setWorldViewCenter(m_hero.getExistence().getPosition() * 80);
    // m_state.update();
    // update_field_of_view();

    // m_root_console.clear();
    // m_state.render_to(m_root_console);
    // m_console_entity.console().update(m_root_console, m_game->render_manager());
  }

  void WorldScene::updateFieldOfView()
  {
    // m_state.map.grid.clear_visible();
    // m_state.map.grid.compute_field_of_vision(m_state.map.hero().entity.position, 8, gf::Visibility::ShadowCast);
  }

  std::vector<gf::Action *> WorldScene::getActions()
  {
    auto controls = Controls::getControls();
    std::vector<gf::Action *> res;
    res.reserve(controls.size());

    for (const auto &[action_name, key_name_tab] : controls)
    {
      gf::Action *action = new gf::Action(action_name);
      for (const auto key_name : key_name_tab)
      {
        action->addKeycodeKeyControl(gf::Keyboard::getKeycodeFromName(key_name));
      }
      res.push_back(action);
    }

    return res;
  }
}
