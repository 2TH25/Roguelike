#include "WorldScene.h"

#include "RogueCMI.h"
#include "Map.h"

namespace rCMI
{
  constexpr gf::Vector2i world_size = {1200, 800};
  constexpr gf::Vector2i MapSize = {80, 50};

  constexpr int RoomMaxSize = 10;
  constexpr int RoomMinSize = 6;
  constexpr int MaxRooms = 30;
  constexpr int MaxMonstersPerRoom = 2;

  WorldScene::WorldScene(RogueCMI *game) : gf::Scene(world_size), m_game(game)
  {
    setWorldViewSize(world_size);
    setWorldViewCenter(world_size / 2);
    

    updateFieldOfView();
  }

  void WorldScene::doHandleActions(gf::Window &window)
  {
    
  }

  void WorldScene::doUpdate([[maybe_unused]] gf::Time time)
  {
    m_state.update();
    update_field_of_view();

    m_root_console.clear();
    m_state.render_to(m_root_console);
    m_console_entity.console().update(m_root_console, m_game->render_manager());
  }

  void WorldScene::update_field_of_view()
  {
    m_state.map.grid.clear_visible();
    m_state.map.grid.compute_field_of_vision(m_state.map.hero().entity.position, 8, gf::Visibility::ShadowCast);
  }
}
