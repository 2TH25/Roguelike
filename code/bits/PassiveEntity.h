#ifndef PASSIVE_ENTITY_H
#define PASSIVE_ENTITY_H

#include <string>
#include <gf/Color.h>
#include <gf/Vector.h>

namespace rCMI {

    struct PassiveEntity {
        gf::Vector2i position = { 0, 0 };
        char16_t character = ' ';
        gf::Color color = White;
        std::string name;
        bool blocks_movement = false;
    };

}

#endif
