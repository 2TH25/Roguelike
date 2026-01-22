#include "WorldScene.h"
#include "RogueCMI.h"
#include <iostream>
#include "MenuScene.h"
#include <gf/Time.h>
#include "InventoryScene.h"
#include "Actions.h"

namespace rCMI
{
  WorldScene::WorldScene(RogueCMI *game)
      : gf::Scene(view_size),
        m_game(game),
        m_world_entity(game),
        m_actions(getActions()),
        m_timeSinceDeath(gf::Time::Zero),
        m_isActivateInventorie(false)
  {
    setClearColor(gf::Color::Black);
    setWorldViewSize(view_size);

    for (gf::Action *action : m_actions)
      addAction(*action);
  }

  void WorldScene::generateMap(gf::Vector2i size)
  {
    if (size == TestMapSize)
      m_world_entity.generate_board();
    else
      m_world_entity.generate_dungeon(size);

    gf::Vector2i TileVect({TileSize, TileSize});
    setWorldViewCenter(m_world_entity.hero().getExistence().getPosition() * TileSize + TileVect / 2);

    const gf::Texture &textureMort = m_game->resources.getTexture("mort.png");

    for (auto &character : m_world_entity.getCharacters())
      character.setDeadTexture(textureMort);

    addWorldEntity(m_world_entity);
  }

  void WorldScene::doProcessEvent(gf::Event &event)
  {
    switch (event.type)
    {
    case gf::EventType::MouseMoved:
      m_mousePosition = event.mouseCursor.coords;
      break;
    default:
      break;
    }
  }

  void WorldScene::doHandleActions([[maybe_unused]] gf::Window &window)
  {
    Character &heroInEntity = m_world_entity.hero();
    gf::Vector2i world_view_size = getWorldView().getSize();

    bool playerMoved = false;

    if (Controls::isActiveAction("move_up", m_actions)){
      if (heroInEntity.goUp(m_world_entity)) {
        heroInEntity.playAnimation("Up");
        playerMoved = true;
      }
    }
    else if (Controls::isActiveAction("move_down", m_actions)){
      if (heroInEntity.goDown(m_world_entity)) {
        heroInEntity.playAnimation("Down");
        playerMoved = true;
      }
    }
    else if (Controls::isActiveAction("move_right", m_actions)){
      if (heroInEntity.goRight(m_world_entity)) {
        heroInEntity.playAnimation("Right");
        playerMoved = true;
      }
    }
    else if (Controls::isActiveAction("move_left", m_actions)){
      if (heroInEntity.goLeft(m_world_entity)) {
        heroInEntity.playAnimation("Left");
        playerMoved = true;
      }
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

    if (Controls::isActiveAction("fire", m_actions))
    {
      if (heroInEntity.alive())
      {
        const gf::View &view = getWorldView();
        gf::RenderTarget &renderer = m_game->getRenderer();
        gf::Vector2f worldPos = renderer.mapPixelToCoords(m_mousePosition, view);

        gf::Vector2i targetTile;
        targetTile.x = static_cast<int>(worldPos.x / TileSize);
        targetTile.y = static_cast<int>(worldPos.y / TileSize);

        if (shoot(m_world_entity, heroInEntity, targetTile))
          playerMoved = true;
      }
    }

    if (Controls::isActiveAction("ToggleInventory", m_actions))
    {
      if (m_isActivateInventorie)
        m_game->popScene();
      else
      {
        m_game->m_InventoryScene->m_inventory.updateInventory(m_game);
        m_game->pushScene(*(m_game->m_InventoryScene));
      }

      m_isActivateInventorie = !m_isActivateInventorie; 
    }

    if (playerMoved)
    {
      if (m_world_entity.isStairs(heroInEntity.getExistence().getPosition()))
      {
        m_world_entity.nextLevel();
        gf::Vector2i TileVect({TileSize, TileSize});
        setWorldViewCenter(m_world_entity.hero().getExistence().getPosition() * TileSize + TileVect / 2);

        return;
      }
      m_world_entity.EnemyTurns();
    }
  }

  void WorldScene::doUpdate([[maybe_unused]] gf::Time time)
  {
    gf::Vector2i TileVect({TileSize, TileSize});
    setWorldViewCenter(m_world_entity.hero().getExistence().getPosition() * TileSize + TileVect / 2);

    if (!m_world_entity.hero().alive())
    {
      m_timeSinceDeath += time;
      if (m_timeSinceDeath.asSeconds() > 2.0f)
        m_game->replaceScene(m_game->m_MenuScene);
    }

    gf::Vector2f heroGridPos = m_world_entity.hero().getExistence().getPosition();

    auto& items = m_world_entity.m_itemManager.items;
    for (auto it = items.begin(); it != items.end(); )
    {
        gf::Vector2i itemGridPos = {
            static_cast<int>(it->sprite.getPosition().x / TileSize),
            static_cast<int>(it->sprite.getPosition().y / TileSize)
        };

        if (static_cast<int>(heroGridPos.x) == itemGridPos.x && 
            static_cast<int>(heroGridPos.y) == itemGridPos.y)
        {
            if (m_game->m_InventoryScene->m_inventory.addItemToBackpack(it->item, m_game))
            {
                std::cout << "Objet ramassé : " << it->item.m_name << std::endl;
                it = items.erase(it);
                
            }
            else
            {
                ++it;
            }
        }
        else
        {
            ++it;
        }
    }
    
    m_world_entity.hero().update(time);

    for (auto &character : m_world_entity.getCharacters()) {
        character.update(time);
    }
    updateFieldOfView();
  }

  void WorldScene::updateFieldOfView()
  {
    m_world_entity.clearMap();
    m_world_entity.fieldOfVision();
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
        action->addKeycodeKeyControl(gf::Keyboard::getKeycodeFromName(key_name));

      res.push_back(action);
    }
    gf::Action *fire = new gf::Action("fire");
    fire->addMouseButtonControl(gf::MouseButton::Left);
    res.push_back(fire);

    return res;
  }
}
