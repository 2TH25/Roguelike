#include "WorldScene.h"
#include "RogueCMI.h"
#include <iostream>
#include "MenuScene.h"
#include <gf/Time.h>
#include "InventoryScene.h"
#include "Actions.h"
#include <gf/Text.h>
#include "EndMenuScene.h"

namespace rCMI
{
  WorldScene::WorldScene(RogueCMI *game)
      : gf::Scene(view_size),
        m_game(game),
        m_world_entity(game),
        m_hud(game, &m_world_entity),
        m_actions(getActions()),
        m_timeSinceDeath(gf::Time::Zero),
        m_isActivateInventory(false),
        m_isActivateMap(false)
  // m_isActivateMenu(false)
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
    addHudEntity(m_hud);
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
    

    if (m_isActivateInventory || m_isActivateMap)
    {
      if (Controls::isActiveAction("ToggleInventory", m_actions))
      {
        if (m_isActivateInventory)
        {
          m_game->popScene();
          m_isActivateInventory = false;
        }
        else
        {
          m_isActivateMap = false;
          setWorldViewSize({800, 800});
          m_game->m_InventoryScene->m_inventory.updateInventory(m_game);
          m_game->pushScene(*(m_game->m_InventoryScene));
          m_isActivateInventory = true;
        }
        return;
      }
      if (Controls::isActiveAction("showMap", m_actions))
      {
        if (m_isActivateMap)
        {
          setWorldViewSize({800, 800});
          m_isActivateMap = false;
        }
        else  
        {
          if (m_isActivateInventory) {
            m_game->popScene();
            m_isActivateInventory = false;
          }
          setWorldViewSize((m_world_entity.getMap().getSize() + 2) * TileSize);
          m_isActivateMap = true;
        }
      }
      // else if (Controls::isActiveAction("showMenu", m_actions))
      // {
      //   if (m_isActivateMenu)
      //   {}
      //   else
      //   {}

      //   m_isActivateMenu = !m_isActivateMenu;
      // }

      return;
    }

    if (Controls::isActiveAction("ToggleInventory", m_actions))
    {
      m_game->m_InventoryScene->m_inventory.updateInventory(m_game);
      m_game->pushScene(*(m_game->m_InventoryScene));
      m_isActivateInventory = true;
      return;
    }

    if (Controls::isActiveAction("showMap", m_actions))
    {
      setWorldViewSize((m_world_entity.getMap().getSize() + 2) * TileSize);
      m_isActivateMap = true;
      return;
    }

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

    if (playerMoved)
    {

      int chestIndex = m_world_entity.m_chestManager.isChestOnTile(m_game);
      if (chestIndex != -1) // On est sur un coffre
      {
        auto& chest = m_world_entity.m_chestManager.m_chests[chestIndex];
        if (!chest.isOpen) {
          m_game->m_ChestScene.setLoots(chest, chestIndex); 

          if (!m_isActivateChest) {
            m_game->pushScene(m_game->m_ChestScene);
            m_isActivateChest = true;
          }
        } else if (m_isActivateChest) {
          m_game->popScene();
          m_isActivateChest = false;
        }
      }
      else if (m_isActivateChest) // On n'est pas sur un coffre mais la scène est active
      {
          m_game->popScene();
          m_isActivateChest = false;
      }


      if (m_world_entity.isStairs(heroInEntity.getExistence().getPosition()))
      {
        m_world_entity.nextLevel();
        gf::Vector2i TileVect({TileSize, TileSize});
        setWorldViewCenter(m_world_entity.hero().getExistence().getPosition() * TileSize + TileVect / 2);

        return;
      } 
      if (m_world_entity.isHealing(heroInEntity.getExistence().getPosition()))
      {
        if (m_world_entity.usHealing(heroInEntity.getExistence().getPosition()))
        {
          heroInEntity.heal(10);
          std::cout << "Le heros a ete soigne de 10 PV !" << std::endl; // Optionnel pour debug
        }
      }
      m_world_entity.EnemyTurns();
    
      
    }
  }

  void WorldScene::doUpdate([[maybe_unused]] gf::Time time)
  {
    gf::Vector2i TileVect({TileSize, TileSize});
    gf::Vector2i playerCenter = m_world_entity.hero().getExistence().getPosition() * TileSize + TileVect / 2;
    gf::Vector2i worldCenter = m_world_entity.getMap().getSize() * TileSize / 2;
    setWorldViewCenter(m_isActivateMap ? worldCenter : playerCenter);

    if (!m_world_entity.hero().alive())
    {
      m_timeSinceDeath += time;
      if (m_timeSinceDeath.asSeconds() > 2.0f)
        m_game->replaceScene(m_game->m_EndMenuScene);
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

  void WorldScene::reset() {
    m_timeSinceDeath = gf::Time::Zero;
    m_isActivateInventory = false;
    m_isActivateMap = false;
    
    m_world_entity.reset(); 
    
    generateMap(MapSize); 
  }
}
