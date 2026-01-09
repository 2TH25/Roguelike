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
  , mov_up("mov_up")
  , mov_down("mov_down")
  , mov_right("mov_down")
  , mov_left("mov_down")
  {
    setClearColor(gf::Color::Black);
    setWorldViewSize(view_size);
    
    setWorldViewCenter(m_map.hero().getExistence().getPosition() * 80);

    mov_up.addScancodeKeyControl(gf::Scancode::W);
    addAction(mov_up);

    mov_down.addScancodeKeyControl(gf::Scancode::S);
    addAction(mov_down);

    mov_right.addScancodeKeyControl(gf::Scancode::D);
    addAction(mov_right);

    mov_left.addKeycodeKeyControl(gf::Keycode::Q);
    addAction(mov_left);

    addWorldEntity(m_map);

    addWorldEntity(m_map);
    // addWorldEntity(m_hero);
    // addWorldEntity(m_slime);
    // addWorldEntity(m_skeleton);
    // addWorldEntity(m_zombie);


    const gf::Texture& textureMort = m_game->resources.getTexture("mort.png");

    for (auto& character : m_map.getCharacters()) {
        character.setDeadTexture(textureMort);
    }
  }

  void WorldScene::doHandleActions(gf::Window &window)
    {
      Character& heroInMap = m_map.hero();

      bool playerMoved = false;

      if (mov_up.isActive()) {
        heroInMap.goUp(m_map);
        playerMoved = true;
      }
      else if (mov_down.isActive()) {
        heroInMap.goDown(m_map);
        playerMoved = true;
      }
      else if (mov_right.isActive()) {
        heroInMap.goRight(m_map);
        playerMoved = true;
      }
      else if (mov_left.isActive()) {
        heroInMap.goLeft(m_map);
        playerMoved = true;
      }

      if (playerMoved) {
          m_map.EnemyTurns();
      }
  }
    

void WorldScene::doUpdate([[maybe_unused]] gf::Time time)
  {
    setWorldViewCenter(m_map.hero().getExistence().getPosition() * 80);
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

  gf::ActionContainer WorldScene::getActions()
  {
    gf::ActionContainer res;
    std::map<std::string, const char *> controls = Controls::getControls();

    // for (const auto &[action_name, key_name] : controls)
    // {
    //   gf::Action newAction(action_name);
    //   newAction.addScancodeKeyControl(gf::Keyboard::getScancodeFromName(key_name));
    //   res.addAction(newAction);
    //   addAction(newAction);
    // }

    return res;
  }
}
