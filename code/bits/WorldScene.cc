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
  , m_slime(Character::slime({5, 4}, game->resources.getTexture("slime.png")))
  , m_skeleton(Character::skeleton({6,7}, game->resources.getTexture("squelette.png")))
  , m_zombie(Character::zombie({8, 2}, game->resources.getTexture("zombie.png")))
  , m_actions(getActions())
  , mov_up("mov_up")
  , mov_down("mov_down")
  , mov_right("mov_down")
  , mov_left("mov_down")
  {
    setClearColor(gf::Color::Black);
    setWorldViewSize(view_size);
    setWorldViewCenter(m_hero.getExistence().getPosition() * 80);

    mov_up.addScancodeKeyControl(gf::Scancode::W);
    addAction(mov_up);

    mov_down.addScancodeKeyControl(gf::Scancode::S);
    addAction(mov_down);

    mov_right.addScancodeKeyControl(gf::Scancode::D);
    addAction(mov_right);

    mov_left.addKeycodeKeyControl(gf::Keycode::Q);
    addAction(mov_left);

    m_map.getCharacters().push_back(m_hero); 
    m_map.getCharacters().push_back(m_slime);
    m_map.getCharacters().push_back(m_skeleton);
    m_map.getCharacters().push_back(m_zombie);

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

  if (mov_up.isActive())
    heroInMap.goUp(m_map);
  else if (mov_down.isActive())
    heroInMap.goDown(m_map);
  else if (mov_right.isActive())
    heroInMap.goRight(m_map);
  else if (mov_left.isActive())
    heroInMap.goLeft(m_map);
}
  void WorldScene::doUpdate([[maybe_unused]] gf::Time time)
  {
    setWorldViewCenter(m_hero.getExistence().getPosition() * 80);
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
