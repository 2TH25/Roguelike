#include "WorldScene.h"

#include "RogueCMI.h"
#include "Map.h"
#include <gf/Rect.h>

#include <gf/Vector.h>
#include <gf/Color.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Entity.h>

#include <iostream>

namespace rCMI
{

  Player::Player()
  : gf::Entity(1)
  {}

  void Player::render([[maybe_unused]] gf::RenderTarget& target, [[maybe_unused]] const gf::RenderStates& states)
  {
    gf::RectangleShape shape({1000, 1000});
    shape.setPosition({0, 0});
    shape.setColor(gf::Color::Red);
    shape.setAnchor(gf::Anchor::Center);
    target.draw(shape);
  }

  WorldScene::WorldScene(RogueCMI *game) 
  : gf::Scene(frame_size)
  , m_game(game)
  , m_player()
  {
    setClearColor(gf::Color::Black);
    setWorldViewSize(frame_size);
    setWorldViewCenter({0, 0});

    addWorldEntity(m_player);

    updateFieldOfView();
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
}

