#include "Item.h"
#include "RogueCMI.h"

namespace rCMI
{

  Item Item::generateRandomItem(RogueCMI *game)
  {

    std::vector<ItemType> types = {
        ItemType::Head, ItemType::Torso, ItemType::Legs,
        ItemType::Hand, ItemType::Boots, ItemType::Accessory, ItemType::Weapon};

    ItemType randomType = types[rand() % types.size()];
    Item newItem;

    switch (randomType)
    {
    case ItemType::Head: {
      Stat s(10, 2, 0); 
      newItem = Item("Casque", ItemType::Head, game->resources.getTexture("Casque.png"), "Heaume Sanguinolent");
      newItem.m_description = "Un casque duquel dégage une puissante odeur de sang... \n Fait partie de l'ensemble Sanguinolent";
      newItem.m_stat = s;
      break;
    }
    case ItemType::Torso: {
      Stat s(30, 5, 0); 
      newItem = Item("Plastron", ItemType::Torso, game->resources.getTexture("Armure.png"),"Plastron Sanguinolent");
      newItem.m_description = "Ce plastron brille d'une lueur rouge qui éveille en vous une envie de tuer... \n Fait partie de l'ensemble Sanguinolent";
      newItem.m_stat = s;
      break;
    }
    case ItemType::Legs: {
      Stat s(10, 2, 0); 
      newItem = Item("Jambieres", ItemType::Legs, game->resources.getTexture("Jambiere.png"), "Jambières Sanguinolentes");
      newItem.m_description = "Des jambières qui semblent maudites... \n Fait partie de l'ensemble Sanguinolent";
      newItem.m_stat = s;
      break;
    }
    case ItemType::Hand: {
      Stat s(0, 2, 3); 
      newItem = Item("Gants", ItemType::Hand, game->resources.getTexture("Gants.png"), "Gants Sanguinolents");
      newItem.m_description = "Vous sentez votre sang bouilloner au contact de ces gants... \n Fait partie de l'ensemble Sanguinolent";
      newItem.m_stat = s;
      break;
    }
    case ItemType::Boots: {
      Stat s(10, 2, 0); 
      newItem = Item("Bottes", ItemType::Boots, game->resources.getTexture("Bottes.png"), "Bottes Sanguinolents");
      newItem.m_description = "Ces bottes semblent porter le lourd fardeau du pêché... \n Fait partie de l'ensemble Sanguinolent";
      newItem.m_stat = s;
      break;
    }
    case ItemType::Accessory: {
      Stat s(0, 2, 10); 
      newItem = Item("Accessoire", ItemType::Accessory, game->resources.getTexture("Bracelet.png"), "Bracelet Sanguinolent");
      newItem.m_description = "Ce bracelet résonne en vous, et vous fait vibrer du plius profond de votre être... \n Fait partie de l'ensemble Sanguinolent";
      newItem.m_stat = s;
      break;
    }

    case ItemType::Weapon: {
      Stat s(10, 0, 0); 
      newItem = Item("Arme", ItemType::Weapon, game->resources.getTexture("Arme.png"), "Tranchant Sanguinolent");
      newItem.m_description = "Une épée qu'on dit capable de scinder tout ce qui est constitué de sang... \n Fait partie de l'ensemble Sanguinolent";
      newItem.m_stat = s;
      break;
    }
    default:
      break;
    }

    return newItem;
  }
}