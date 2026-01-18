#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <gf/Texture.h>


namespace rCMI {

    enum class ItemType {
        Head, Torso, Legs, Hand, Feet, Accessory, Consumable, Misc
    };



    class Item {

        public:
            std::string m_name;
            std::string m_description;
            ItemType m_type;
            const gf::Texture* m_texture = nullptr;



            Item(std::string name, ItemType type, const gf::Texture& tex) 
            : m_name(name), m_type(type), m_texture(&tex) {}

            void setTexture(gf::Texture texture) {
                this->m_texture = &texture;
            }

    };
}

#endif