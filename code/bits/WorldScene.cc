#include "WorldScene.h"

#include "RogueCMI.h"
#include <iostream>

namespace rCMI
{
  WorldScene::WorldScene(RogueCMI *game)
  : gf::Scene(view_size)
  , m_game(game)
  , m_map(game)
  , m_hero(Character::hero({5, 5}, game->resources.getTexture("perso70.png")))
  , m_actions(getActions())
  {
    setClearColor(gf::Color::Black);
    setWorldViewSize(view_size);
    setWorldViewCenter({view_size.x / 2, view_size.y / 2});

    addWorldEntity(m_map);
    addWorldEntity(m_hero);
  }

  void WorldScene::doHandleActions(gf::Window &window)
  {

  }

  void WorldScene::doUpdate([[maybe_unused]] gf::Time time)
  {
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

  gf::ActionContainer WorldScene::getActions()
  {
    gf::ActionContainer res;
    std::map<std::string, std::string> controls = Controls::getControls();

    // for (const auto &[action_name, key_name] : controls)
    // {
      
    // }

    return res;
  }
}
