#ifndef ENTITY_H
#define ENTITY_H

#include <string>

#include <gf/Color.h>
#include <gf/Vector.h>

namespace rCMI {

  struct Entity {
    gf::Vector2i position = { 0, 0 };
    char16_t character = ' ';
    gf::Color color = White;
    std::string name;
    bool blocks_movement = false;
  };

}

#endif // ENTITY_H