#include "Item.h"
#include "RogueCMI.h"

namespace rCMI
{

  Item Item::generateRandomItem(RogueCMI *game)
  {



    int roll = rand() % 100;
    Rarity selectedRarity;

    if (roll < 50)       selectedRarity = Rarity::Common;
    else if (roll < 75)  selectedRarity = Rarity::Uncommon;
    else if (roll < 90)  selectedRarity = Rarity::Rare;
    else if (roll < 98)  selectedRarity = Rarity::Epic;
    else                 selectedRarity = Rarity::Legendary;

    //std::string name, std::string id, ItemType type, Rarity rarity, gf::Texture& tex, std::string desc, Stat s
    
    Item newItem;

    switch (selectedRarity)
    {
      case Rarity::Legendary: {
        int pick = rand() % 7;
        switch (pick) {
              case 0:
                newItem = Item( std::string("Heaume Sanguinolent"),std::string("Casque"), ItemType::Head, Rarity::Legendary,
                game->resources.getTexture("Casque.png"),std::string("Un casque duquel dégage une puissante odeur de sang... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(10, 2, 0));
                break;

              case 1:
                newItem = Item( std::string("Plastron Sanguinolent"),std::string("Armure"), ItemType::Torso, Rarity::Legendary,
                game->resources.getTexture("Armure.png"),std::string("Ce plastron brille d'une lueur rouge qui éveille en vous une envie de tuer... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(30, 5, 0));
                break;

              case 2:
                newItem = Item( std::string("Jambières Sanguinolentes"),std::string("Jambiere"), ItemType::Legs, Rarity::Legendary,
                game->resources.getTexture("Jambiere.png"),std::string("Des jambières qui semblent maudites... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(10, 2, 0));
                break;

              case 3:
                newItem = Item( std::string("Gants Sanguinolents"),std::string("Gants"), ItemType::Hand, Rarity::Legendary,
                game->resources.getTexture("Gants.png"),std::string("Vous sentez votre sang bouilloner au contact de ces gants... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(0, 2, 3));
                break;

              case 4:
                newItem = Item( std::string("Bottes Sanguinolentes"),std::string("Bottes"), ItemType::Boots, Rarity::Legendary,
                game->resources.getTexture("Bottes.png"),std::string("Ces bottes semblent porter le lourd fardeau du pêché... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(10, 2, 0));
                break;

              case 5:
                newItem = Item( std::string("Bracelet Sanguinolent"),std::string("Bracelet"), ItemType::Accessory, Rarity::Legendary,
                game->resources.getTexture("Bracelet.png"),std::string("Ce bracelet résonne en vous, et vous fait vibrer du plus profond de votre être... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(10, 2, 0));
                break;

              case 6:
                newItem = Item( std::string("Tranchant Sanguinolent"),std::string("Arme"), ItemType::Weapon, Rarity::Legendary,
                game->resources.getTexture("Arme.png"),std::string("Une épée qu'on dit capable de scinder tout ce qui est constitué de sang... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(0, 2, 10));
                break;

        } 
        break;
      } // fin des Légendaires

      case Rarity::Epic: {

        int pick = rand() % 7;
        switch (pick) {
              case 0:
                newItem = Item( std::string("Heaume Clinquant"),std::string("CasqueOr"), ItemType::Head, Rarity::Epic,
                game->resources.getTexture("CasqueOr.png"),std::string("Un casque duquel dégage une puissante odeur de sang... \n Fait partie de l'ensemble Clinquant"),
                Stat(10, 2, 0));
                break;

              case 1:
                newItem = Item( std::string("Plastron Clinquant"),std::string("PlastronOr"), ItemType::Torso, Rarity::Epic,
                game->resources.getTexture("PlastronOr.png"),std::string("Ce plastron brille d'une lueur rouge qui éveille en vous une envie de tuer... \n Fait partie de l'ensemble Clinquant"),
                Stat(30, 5, 0));
                break;

              case 2:
                newItem = Item( std::string("Jambières Clinquantes"),std::string("JambiereOr"), ItemType::Legs, Rarity::Epic,
                game->resources.getTexture("JambiereOr.png"),std::string("Des jambières qui semblent maudites... \n Fait partie de l'ensemble Clinquant"),
                Stat(10, 2, 0));
                break;

              case 3:
                newItem = Item( std::string("Gants Clinquants"),std::string("GantOr"), ItemType::Hand, Rarity::Epic,
                game->resources.getTexture("GantOr.png"),std::string("Vous sentez votre sang bouilloner au contact de ces gants... \n Fait partie de l'ensemble Clinquant"),
                Stat(0, 2, 3));
                break;

              case 4:
                newItem = Item( std::string("Bottes Clinquantes"),std::string("BotteOr"), ItemType::Boots, Rarity::Epic,
                game->resources.getTexture("BotteOr.png"),std::string("Ces bottes semblent porter le lourd fardeau du pêché... \n Fait partie de l'ensemble Clinquant"),
                Stat(10, 2, 0));
                break;

              case 5:
                newItem = Item( std::string("Bracelet Clinquant"),std::string("BraceletOr"), ItemType::Accessory, Rarity::Epic,
                game->resources.getTexture("BraceletOr.png"),std::string("Ce bracelet résonne en vous, et vous fait vibrer du plius profond de votre être... \n Fait partie de l'ensemble Clinquant"),
                Stat(10, 2, 0));
                break;

              case 6:
                newItem = Item( std::string("Découpe Éclatante"),std::string("ArmeOr"), ItemType::Weapon, Rarity::Epic,
                game->resources.getTexture("ArmeOr.png"),std::string("Une épée qu'on dit capable de scinder tout ce qui est constitué de sang... \n Fait partie de l'ensemble Clinquant"),
                Stat(0, 2, 10));
                break;

        } 
        break;
      } // Fin des Epiques

      case Rarity::Rare: {

        int pick = rand() % 7;
        switch (pick) {
              case 0:
                newItem = Item( std::string("Heaume Argenté"),std::string("CasqueBleu"), ItemType::Head, Rarity::Rare,
                game->resources.getTexture("Casque.png"),std::string("Un casque duquel dégage une puissante Uncommonodeur de sang... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(10, 2, 0));
                break;

              case 1:
                newItem = Item( std::string("Plastron Argenté"),std::string("PlastronBleu"), ItemType::Torso, Rarity::Rare,
                game->resources.getTexture("PlastronBleu.png"),std::string("Ce plastron brille d'une lueur rouge qui éveille en vous une envie de tuer... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(30, 5, 0));
                break;

              case 2:
                newItem = Item( std::string("Jambières Argentées"),std::string("JambiereBleu"), ItemType::Legs, Rarity::Rare,
                game->resources.getTexture("JambiereBleu.png"),std::string("Des jambières qui semblent maudites... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(10, 2, 0));
                break;

              case 3:
                newItem = Item( std::string("Gants Argentés"),std::string("GantBleu"), ItemType::Hand, Rarity::Rare,
                game->resources.getTexture("GantBleu.png"),std::string("Vous sentez votre sang bouilloner au contact de ces gants... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(0, 2, 3));
                break;

              case 4:
                newItem = Item( std::string("Bottes Argentées"),std::string("BotteBleu"), ItemType::Boots, Rarity::Rare,
                game->resources.getTexture("BotteBleu.png"),std::string("Ces bottes semblent porter le lourd fardeau du pêché... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(10, 2, 0));
                break;

              case 5:
                newItem = Item( std::string("Bracelet Argenté"),std::string("BraceletBleu"), ItemType::Accessory, Rarity::Rare,
                game->resources.getTexture("BraceletBleu.png"),std::string("Ce bracelet résonne en vous, et vous fait vibrer du plius profond de votre être... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(10, 2, 0));
                break;

              case 6:
                newItem = Item( std::string("Tranchant D'Argent"),std::string("ArmeBleu"), ItemType::Weapon, Rarity::Rare,
                game->resources.getTexture("ArmeBleu.png"),std::string("Une épée qu'on dit capable de scinder tout ce qui est constitué de sang... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(0, 2, 10));
                break;
        } 
        break;
      } // Fin des Rare

      case Rarity::Uncommon: {

        int pick = rand() % 7;
        switch (pick) {
              case 0:
                newItem = Item( std::string("Heaume Boisé"),std::string("CasqueVert"), ItemType::Head, Rarity::Uncommon,
                game->resources.getTexture("CasqueVert.png"),std::string("Un casque duquel dégage une puissante odeur de sang... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(10, 2, 0));
                break;

              case 1:
                newItem = Item( std::string("Plastron Boisé"),std::string("PlastronVert"), ItemType::Torso, Rarity::Uncommon,
                game->resources.getTexture("PlastronVert.png"),std::string("Ce plastron brille d'une lueur rouge qui éveille en vous une envie de tuer... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(30, 5, 0));
                break;

              case 2:
                newItem = Item( std::string("Jambières Boisées"),std::string("JambiereVert"), ItemType::Legs, Rarity::Uncommon,
                game->resources.getTexture("JambiereVert.png"),std::string("Des jambières qui semblent maudites... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(10, 2, 0));
                break;

              case 3:
                newItem = Item( std::string("Gants Boisés"),std::string("GantVert"), ItemType::Hand, Rarity::Uncommon,
                game->resources.getTexture("GantVert.png"),std::string("Vous sentez votre sang bouilloner au contact de ces gants... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(0, 2, 3));
                break;

              case 4:
                newItem = Item( std::string("Bottes Boisées"),std::string("BotteVert"), ItemType::Boots, Rarity::Uncommon,
                game->resources.getTexture("BotteVert.png"),std::string("Ces bottes semblent porter le lourd fardeau du pêché... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(10, 2, 0));
                break;

              case 5:
                newItem = Item( std::string("Bracelet Boisé"),std::string("BraceletVert"), ItemType::Accessory, Rarity::Uncommon,
                game->resources.getTexture("BraceletVert.png"),std::string("Ce bracelet résonne en vous, et vous fait vibrer du plius profond de votre être... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(10, 2, 0));
                break;

              case 6:
                newItem = Item( std::string("Taillade"),std::string("ArmeVert"), ItemType::Weapon, Rarity::Uncommon,
                game->resources.getTexture("ArmeVert.png"),std::string("Une épée qu'on dit capable de scinder tout ce qui est constitué de sang... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(0, 2, 10));
                break;
        } 
        break;
      } // Fin des Uncommon

      case Rarity::Common: {

        newItem = Item( std::string("Potion de soin"),std::string("ItemSoin"), ItemType::Consumable, Rarity::Common,
                game->resources.getTexture("ItemSoin.png"),std::string("Une épée qu'on dit capable de scinder tout ce qui est constitué de sang... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(20,0,0));
                break;
        
      } // Fin des Common

    }
    return newItem;
  }
}