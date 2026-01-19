#include "Item.h"
#include "RogueCMI.h"

namespace rCMI {



    Item Item::generateRandomItem(RogueCMI *game) {

        std::vector<ItemType> types = { 
            ItemType::Head, ItemType::Torso, ItemType::Legs, 
            ItemType::Hand, ItemType::Feet, ItemType::Accessory 
        };

        ItemType randomType = types[rand() % types.size()];
        std::string name;
        std::string textureKey;

        switch (randomType) {
            case ItemType::Head:      name = "Casque de test"; textureKey = "Casque.jpg"; break;
            case ItemType::Torso:     name = "Plastron de test"; textureKey = "Plastron.png"; break;
            case ItemType::Legs:      name = "Jambieres de test"; textureKey = "Jambieres.jpg"; break;
            case ItemType::Hand:      name = "Gants de test"; textureKey = "Gants.jpg"; break;
            case ItemType::Feet:      name = "Bottes de test"; textureKey = "Bottes.jpg"; break;
            case ItemType::Accessory: name = "Anneau de test"; textureKey = "accessoire.jpg"; break;
        }

        return Item(name, randomType, game->resources.getTexture(textureKey));
    }
}