#include "WorldScene.h"

#include "RogueCMI.h"
#include <iostream>
#include "MenuScene.h"
#include <gf/Time.h>
#include "InventoryScene.h"

namespace rCMI
{
  WorldScene::WorldScene(RogueCMI *game)
      : gf::Scene(view_size),
        m_game(game),
        m_map(game),
        m_actions(getActions()),
        m_timeSinceDeath(gf::Time::Zero),
        m_gameOverHandled(false),
        toggleInventory("ToggleInventory")
  {
    setClearColor(gf::Color::Black);
    setWorldViewSize(view_size);

    for (gf::Action *action : m_actions)
      addAction(*action);

    toggleInventory.addKeycodeKeyControl(gf::Keycode::Tab);
    addAction(toggleInventory);
  }

  void WorldScene::generateMap(gf::Vector2i size)
  {
    m_map.generate_dungeon(size);

    gf::Vector2i TileVect({TileSize, TileSize});
    setWorldViewCenter(m_map.hero().getExistence().getPosition() * TileSize + TileVect / 2);

    const gf::Texture &textureMort = m_game->resources.getTexture("mort.png");

    for (auto &character : m_map.getCharacters())
      character.setDeadTexture(textureMort);

    addWorldEntity(m_map);
  }

  void WorldScene::doHandleActions([[maybe_unused]] gf::Window &window)
  {
    Character &heroInMap = m_map.hero();
    gf::Vector2i world_view_size = getWorldView().getSize();

    bool playerMoved = false;

    if (Controls::isActiveAction("move_up", m_actions))
    {
      playerMoved = heroInMap.goUp(m_map);
    }
    else if (Controls::isActiveAction("move_down", m_actions))
    {
      playerMoved = heroInMap.goDown(m_map);
    }
    else if (Controls::isActiveAction("move_right", m_actions))
    {
      playerMoved = heroInMap.goRight(m_map);
    }
    else if (Controls::isActiveAction("move_left", m_actions))
    {
      playerMoved = heroInMap.goLeft(m_map);
    }

    if (Controls::isActiveAction("zoom_cam", m_actions) && world_view_size.x > 300)
    {
      if (world_view_size.x / 1.5 < 6000 && world_view_size.x > 6000)
      {
        std::cout << "Changement de texture bien\n"; // TODO: afficher avec les textures normales
      }
      setWorldViewSize(world_view_size / 1.5);
    }
    else if (Controls::isActiveAction("unzoom_cam", m_actions) && world_view_size.x < 25000)
    {
      if (world_view_size.x * 1.5 > 6000 && world_view_size.x < 6000)
      {
        std::cout << "Changement de texture null\n"; // TODO: afficher avec les textures amoindrie
      }
      setWorldViewSize(world_view_size * 1.5);
    }

    if (playerMoved)
    {
      if (m_map.isStairs(heroInMap.getExistence().getPosition())) 
      {
          m_map.nextLevel();          
          gf::Vector2i TileVect({TileSize, TileSize});
          setWorldViewCenter(m_map.hero().getExistence().getPosition() * TileSize + TileVect / 2);
          
          return; 
      }
      m_map.EnemyTurns();
    }
  }

  void WorldScene::doUpdate([[maybe_unused]] gf::Time time)
  {
    gf::Vector2i TileVect({TileSize, TileSize});
    setWorldViewCenter(m_map.hero().getExistence().getPosition() * TileSize + TileVect / 2);

    if (!m_map.hero().alive())
    {
      m_timeSinceDeath += time;
      if (m_timeSinceDeath.asSeconds() > 2.0f && !m_gameOverHandled)
      {
        m_gameOverHandled = true;
        m_game->replaceScene(m_game->m_MenuScene);
      }
    }

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
