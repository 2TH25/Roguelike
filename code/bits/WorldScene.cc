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
      for (gf::Action* action : m_actions) {
        addAction(*action);
        if (action->getName() == "fire") {
            m_fireAction = action;
        }
      }  

    toggleInventory.addKeycodeKeyControl(gf::Keycode::Tab);
    addAction(toggleInventory);
  }

  void WorldScene::generateMap(gf::Vector2i size)
  {
    if (size == TestMapSize)
      m_map.generate_board();
    else
      m_map.generate_dungeon(size);

    gf::Vector2i TileVect({TileSize, TileSize});
    setWorldViewCenter(m_map.hero().getExistence().getPosition() * TileSize + TileVect / 2);

    const gf::Texture &textureMort = m_game->resources.getTexture("mort.png");

    for (auto &character : m_map.getCharacters())
      character.setDeadTexture(textureMort);

    addWorldEntity(m_map);
    addWorldEntity(m_itemManager);

    int numberOfItems = 10;
    
    for (int i = 0; i < numberOfItems; ++i) {
      bool found = false;
      int x_final;
      int y_final;
      do {
        int x = rand() % size.x;
        int y = rand() % size.y;

        if(m_map.isWalkable({x,y})) {
          found = true;
            x_final = x;
            y_final = y;
        }

      } while(!found);
        
        gf::Vector2f pixelPos = { x_final * (float)TileSize + TileSize/2.0f, y_final * (float)TileSize + TileSize/2.0f };
        spawnItem(pixelPos);
        
    }
    
    std::cout << "Donjon peuple avec " << m_itemManager.items.size() << " items." << std::endl;
  }

  void WorldScene::doProcessEvent(gf::Event& event)
  {
    switch (event.type) {
      case gf::EventType::MouseMoved:
        m_mousePosition = event.mouseCursor.coords;
        break;
      default:
        break;
    }
    if (event.type == gf::EventType::KeyPressed && event.key.keycode == gf::Keycode::Tab) {
        m_game->m_InventoryScene->m_inventory.updateInventory(m_game);
        m_game->pushScene(*(m_game->m_InventoryScene));
    }
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

    if (m_fireAction && m_fireAction->isActive())
    {
        if (heroInMap.alive()) 
    {
        const gf::View& view = getWorldView();
        gf::RenderTarget& renderer = m_game->getRenderer();
        gf::Vector2f worldPos = renderer.mapPixelToCoords(m_mousePosition, view);
        
        gf::Vector2i targetTile;
        targetTile.x = static_cast<int>(worldPos.x / TileSize);
        targetTile.y = static_cast<int>(worldPos.y / TileSize);

        if (shoot(m_map, heroInMap, targetTile))
        {
             playerMoved = true; 
        }
    }
        m_fireAction->reset();
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

    gf::Vector2f heroGridPos = m_map.hero().getExistence().getPosition();

    for (auto it = m_itemManager.items.begin(); it != m_itemManager.items.end(); ) {
        
        gf::Vector2i itemGridPos = { 
            static_cast<int>(it->sprite.getPosition().x / TileSize), 
            static_cast<int>(it->sprite.getPosition().y / TileSize) 
        };

        if (heroGridPos.x == itemGridPos.x && heroGridPos.y == itemGridPos.y) {
            if (m_game->m_InventoryScene->m_inventory.addItemToBackpack(it->item,m_game)) {
                it = m_itemManager.items.erase(it);
                std::cout << "Objet ramassé !" << std::endl;
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }

    updateFieldOfView();

    // m_root_console.clear();
    // m_state.render_to(m_root_console);
    // m_console_entity.console().update(m_root_console, m_game->render_manager());
  }

  void WorldScene::updateFieldOfView()
  {
    m_map.clearMap();
    m_map.fieldOfVision();
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
    gf::Action *fire = new gf::Action("fire");
    fire->addMouseButtonControl(gf::MouseButton::Left);
    res.push_back(fire);
        
    return res;
  }

  void WorldScene::spawnItem(gf::Vector2f position) {
    Item newItem = Item::generateRandomItem(m_game);

    DroppedItem dropped;
    dropped.item = newItem;
    if (newItem.m_texture) {
        dropped.sprite.setTexture(*newItem.m_texture);
    }
    
    gf::Vector2f size = dropped.sprite.getLocalBounds().getSize();
    
    dropped.sprite.setOrigin({size.x / 2.0f, size.y / 2.0f});
    
    dropped.sprite.setPosition(position);
    dropped.sprite.setScale({0.5f, 0.5f});

    m_itemManager.items.push_back(dropped);
  }
}
