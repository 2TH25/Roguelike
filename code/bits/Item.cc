#include "Item.h"
#include "RogueCMI.h"

namespace rCMI
{

  Item Item::generateRandomItem(RogueCMI *game)
  {

    std::vector<ItemType> types = {
        ItemType::Head, ItemType::Torso, ItemType::Legs,
        ItemType::Hand, ItemType::Boots, ItemType::Accessory};

    ItemType randomType = types[rand() % types.size()];
    Item newItem;

    switch (randomType)
    {
    case ItemType::Head: {
      Stat s(10, 2, 0); 
      newItem = Item("Casque", ItemType::Head, game->resources.getTexture("Casque.png"));
      newItem.m_stat = s;
      break;
    }
    case ItemType::Torso: {
      Stat s(30, 5, 0); 
      newItem = Item("Plastron", ItemType::Torso, game->resources.getTexture("Armure.png"));
      newItem.m_stat = s;
      break;
    }
    case ItemType::Legs: {
      Stat s(10, 2, 0); 
      newItem = Item("Jambieres", ItemType::Legs, game->resources.getTexture("Jambiere.png"));
      newItem.m_stat = s;
      break;
    }
    case ItemType::Hand: {
      Stat s(0, 2, 3); 
      newItem = Item("Gants", ItemType::Hand, game->resources.getTexture("Gants.png"));
      newItem.m_stat = s;
      break;
    }
    case ItemType::Boots: {
      Stat s(10, 2, 0); 
      newItem = Item("Bottes", ItemType::Boots, game->resources.getTexture("Bottes.png"));
      newItem.m_stat = s;
      break;
    }
    case ItemType::Accessory: {
      Stat s(0, 2, 10); 
      newItem = Item("Accessoire", ItemType::Accessory, game->resources.getTexture("Bracelet.png"));
      newItem.m_stat = s;
      break;
    }
    default:
      break;
    }

    return newItem;
  }
}