#include "Item.h"
#include "RogueCMI.h"

namespace rCMI
{

  Item::Item() 
  : m_name("")
  , m_texture(nullptr)
  , m_type(ItemType::None)
{
}

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
                newItem = Item( std::string("Casque Sanguinolent"),std::string("Casque"), ItemType::Head, Rarity::Legendary,
                game->resources.getTexture("Casque.png"),std::string("Un casque dont vous sentez un pouvoir immense\n mais aussi une odeur de sang violente s'en dégage... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(10, 2, 0));
                break;

              case 1:
                newItem = Item( std::string("Plastron Sanguinolent"),std::string("Armure"), ItemType::Torso, Rarity::Legendary,
                game->resources.getTexture("Armure.png"),std::string("Ce plastron brille d'une lueur rouge qui éveille en vous une envie de tuer... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(30, 5, 0));
                break;

              case 2:
                newItem = Item( std::string("Jambières Sanguinolentes"),std::string("Jambiere"), ItemType::Legs, Rarity::Legendary,
                game->resources.getTexture("Jambiere.png"),std::string("Des jambières qui font bouillonner votre sang et vous fait perdre tous vos moyens... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(10, 2, 0));
                break;

              case 3:
                newItem = Item( std::string("Gants Sanguinolents"),std::string("Gants"), ItemType::Hand, Rarity::Legendary,
                game->resources.getTexture("Gants.png"),std::string("Au contact de ces gants, vous sentez un pouvoir de destruction au creux de vos mains... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(0, 2, 3));
                break;

              case 4:
                newItem = Item( std::string("Bottes Sanguinolentes"),std::string("Bottes"), ItemType::Boots, Rarity::Legendary,
                game->resources.getTexture("Bottes.png"),std::string("Ces bottes semblent porter le lourd fardeau du pêché et... De la mort... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(10, 2, 0));
                break;

              case 5:
                newItem = Item( std::string("Bracelet Sanguinolent"),std::string("Bracelet"), ItemType::Accessory, Rarity::Legendary,
                game->resources.getTexture("Bracelet.png"),std::string("Ce bracelet résonne en vous, et vous fait vibrer du plus profond de votre être... \n Fait partie de l'ensemble Sanguinolent"),
                Stat(10, 2, 0));
                break;

              case 6:
                newItem = Item( std::string("Tranchant Sanguinolent"),std::string("Arme"), ItemType::Weapon, Rarity::Legendary,
                game->resources.getTexture("Arme.png"),std::string("Une épée qu'on dit capable de scinder tout ce qui existe en ce monde... \n Fait partie de l'ensemble Sanguinolent"),
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
                game->resources.getTexture("CasqueOr.png"),std::string("Un casque retrouvé dans le désert. \n Protègeait certainement le chef d'un village nomade \n Fait partie de l'ensemble Clinquant"),
                Stat(10, 2, 0));
                break;

              case 1:
                newItem = Item( std::string("Plastron Clinquant"),std::string("PlastronOr"), ItemType::Torso, Rarity::Epic,
                game->resources.getTexture("PlastronOr.png"),std::string("Ce plastron brille d'une lueur dorée qui vous hypnotise... \n Fait partie de l'ensemble Clinquant"),
                Stat(30, 5, 0));
                break;

              case 2:
                newItem = Item( std::string("Jambières Clinquantes"),std::string("JambiereOr"), ItemType::Legs, Rarity::Epic,
                game->resources.getTexture("JambiereOr.png"),std::string("Des jambières d'un doré inoubliable, vous êtes prêts à tout pour les garder auprès de vous... \n Fait partie de l'ensemble Clinquant"),
                Stat(10, 2, 0));
                break;

              case 3:
                newItem = Item( std::string("Gants Clinquants"),std::string("GantOr"), ItemType::Hand, Rarity::Epic,
                game->resources.getTexture("GantOr.png"),std::string("Ces gants vous font penser à une histoire... \n Un homme et des mains en or... Rah, vous avez oublié ce conte... \n Fait partie de l'ensemble Clinquant"),
                Stat(0, 2, 3));
                break;

              case 4:
                newItem = Item( std::string("Bottes Clinquantes"),std::string("BotteOr"), ItemType::Boots, Rarity::Epic,
                game->resources.getTexture("BotteOr.png"),std::string("Ces bottes brillent de milles feux, vous marchez sur des étoiles. \n Fait partie de l'ensemble Clinquant"),
                Stat(10, 2, 0));
                break;

              case 5:
                newItem = Item( std::string("Bracelet Clinquant"),std::string("BraceletOr"), ItemType::Accessory, Rarity::Epic,
                game->resources.getTexture("BraceletOr.png"),std::string("Et si ce bracelet faisait partie des bijoux de la couronne ? \n Fait partie de l'ensemble Clinquant"),
                Stat(10, 2, 0));
                break;

              case 6:
                newItem = Item( std::string("Découpe Éclatante"),std::string("ArmeOr"), ItemType::Weapon, Rarity::Epic,
                game->resources.getTexture("ArmeOr.png"),std::string("Une épée qui semble infligée beaucoup de dégats...\n Fait partie de l'ensemble Clinquant"),
                Stat(0, 2, 10));
                break;

        } 
        break;
      } // Fin des Epiques

      case Rarity::Rare: {

        int pick = rand() % 8;
        switch (pick) {
              case 0:
                newItem = Item( std::string("Heaume Argenté"),std::string("CasqueBleu"), ItemType::Head, Rarity::Rare,
                game->resources.getTexture("CasqueBleu.png"),std::string("Un casque dont un pouvoir de protection semble jaillir... \n  Vous vous sentez protéger auprès de ce casque. \n Fait partie de l'ensemble Argenté"),
                Stat(10, 2, 0));
                break;

              case 1:
                newItem = Item( std::string("Plastron Argenté"),std::string("PlastronBleu"), ItemType::Torso, Rarity::Rare,
                game->resources.getTexture("PlastronBleu.png"),std::string("Ce plastron brille d'une lueur bleu d'une élégance fascinante. \n Fait partie de l'ensemble Argenté"),
                Stat(30, 5, 0));
                break;

              case 2:
                newItem = Item( std::string("Jambières Argentées"),std::string("JambiereBleu"), ItemType::Legs, Rarity::Rare,
                game->resources.getTexture("JambiereBleu.png"),std::string("Des jambières argentées, qui semblent aussi solide qu'un soldat des temps anciens. \n Fait partie de l'ensemble Argenté"),
                Stat(10, 2, 0));
                break;

              case 3:
                newItem = Item( std::string("Gants Argentés"),std::string("GantBleu"), ItemType::Hand, Rarity::Rare,
                game->resources.getTexture("GantBleu.png"),std::string("Ces gants semblent rigides... \n Au moins, vous y serez protégé. \n Fait partie de l'ensemble Argenté"),
                Stat(0, 2, 3));
                break;

              case 4:
                newItem = Item( std::string("Bottes Argentées"),std::string("BotteBleu"), ItemType::Boots, Rarity::Rare,
                game->resources.getTexture("BotteBleu.png"),std::string("Ces bottes sont lourdes et un bleu majestueux en sort...\n  Fait partie de l'ensemble Argenté"),
                Stat(10, 2, 0));
                break;

              case 5:
                newItem = Item( std::string("Bracelet Argenté"),std::string("BraceletBleu"), ItemType::Accessory, Rarity::Rare,
                game->resources.getTexture("BraceletBleu.png"),std::string("Ce bracelet est magnifique, vous pourriez porter ça dans votre vie de tous les jours. \n Peu importe votre âge. \n Fait partie de l'ensemble Argenté"),
                Stat(10, 2, 0));
                break;

              case 6:
                newItem = Item( std::string("Tranchant D'Argent"),std::string("ArmeBleu"), ItemType::Weapon, Rarity::Rare,
                game->resources.getTexture("ArmeBleu.png"),std::string("Une épée qu'on dit capable de trancher des monstres, attention à vos doigts... \n Fait partie de l'ensemble Argenté"),
                Stat(0, 2, 10));
                break;


              case 7:
                newItem = Item( std::string("Arc"),std::string("Arc"), ItemType::Bow, Rarity::Rare,
                  game->resources.getTexture("Arc.png"),std::string("Un arc tout ce qu'il y a de plus banal, mais qui a le mérite de pouvoir blesser à distance. \n (Cliquez sur l'ennemi pour tirer) \n (Nécessite des flèches pour son utilisation)."),
                  Stat(0,0,0));
                  break;
        } 
        break;
      } // Fin des Rare

      case Rarity::Uncommon: {

        int pick = rand() % 7;
        switch (pick) {
              case 0:
                newItem = Item( std::string("Heaume Boisé"),std::string("CasqueVert"), ItemType::Head, Rarity::Uncommon,
                game->resources.getTexture("CasqueVert.png"),std::string("Un casque orné d'une pierre verte \n Vous pensez directement au roi de la forêt. \n Fait partie de l'ensemble Boisé"),
                Stat(10, 2, 0));
                break;

              case 1:
                newItem = Item( std::string("Plastron Boisé"),std::string("PlastronVert"), ItemType::Torso, Rarity::Uncommon,
                game->resources.getTexture("PlastronVert.png"),std::string("Ce plastron léger et beau vous rassure. \n C'est mieux que rien... \n Fait partie de l'ensemble Boisé"),
                Stat(30, 5, 0));
                break;

              case 2:
                newItem = Item( std::string("Jambières Boisées"),std::string("JambiereVert"), ItemType::Legs, Rarity::Uncommon,
                game->resources.getTexture("JambiereVert.png"),std::string("Des jambières qui ne vous protègent pas vraiment... \n Mais qui vous donnent un air de véritable aventurier... \n Fait partie de l'ensemble Boisé"),
                Stat(10, 2, 0));
                break;

              case 3:
                newItem = Item( std::string("Gants Boisés"),std::string("GantVert"), ItemType::Hand, Rarity::Uncommon,
                game->resources.getTexture("GantVert.png"),std::string("Ces gants ornés d'une pierre verte vous donnent une sensation de tranquilité.\n Vous avez moins mal aux mains... \nFait partie de l'ensemble Boisé"),
                Stat(0, 2, 3));
                break;

              case 4:
                newItem = Item( std::string("Bottes Boisées"),std::string("BotteVert"), ItemType::Boots, Rarity::Uncommon,
                game->resources.getTexture("BotteVert.png"),std::string("Ces bottes légères vous donnent l'impression de courir plus vite, en pleine nature ! \n Fait partie de l'ensemble Boisé"),
                Stat(10, 2, 0));
                break;

              case 5:
                newItem = Item( std::string("Bracelet Boisé"),std::string("BraceletVert"), ItemType::Accessory, Rarity::Uncommon,
                game->resources.getTexture("BraceletVert.png"),std::string("Ce bracelet bas de gamme vous déçoit, mais bon... \n Fait partie de l'ensemble Boisé"),
                Stat(10, 2, 0));
                break;

              case 6:
                newItem = Item( std::string("Taillade"),std::string("ArmeVert"), ItemType::Weapon, Rarity::Uncommon,
                game->resources.getTexture("ArmeVert.png"),std::string("Une épée en bois mais plus robuste qu'une branche ! \n Fait partie de l'ensemble Boisé"),
                Stat(0, 2, 10));
                break;
        } 
        break;
      } // Fin des Uncommon

      case Rarity::Common: {

        int pick = rand() % 2;
        switch (pick) {
          case 0:
            newItem = Item( std::string("Potion de soin"),std::string("ItemSoin"), ItemType::Consumable, Rarity::Common,
                    game->resources.getTexture("ItemSoin.png"),std::string("Une potion de soin classique. \n Restaure 20 points de vie."),
                    Stat(20,0,0));
                    break;

          case 1:
            newItem = Item( std::string("Fleche"),std::string("Fleche"), ItemType::Arrow, Rarity::Common,
                    game->resources.getTexture("Fleche.png"),std::string("Une flèche de qualité médiocre, qui permettra sûrement d'atteindre vos ennemis à distance. \n (Une flèche sera consommée par tir)."),
                    Stat(0,0,0));
                    break;

        }
        break;
        
      } // Fin des Common

    }
    return newItem;
  }
}