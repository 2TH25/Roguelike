#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <gf/Color.h>
#include <gf/Vector.h>

namespace rCMI {

  struct Entity {
    Entity() = default;

    Entity(gf::Vector2i pos, char16_t ch, gf::Color4f col, std::string n, bool blocks)
      : position(pos), character(ch), color(col), name(std::move(n)), blocks_movement(blocks)
    {}

    gf::Vector2i position = { 0, 0 };
    char16_t character = u' ';
    gf::Color4f color = gf::Color::White; 
    std::string name;
    bool blocks_movement = false;
  };

}

#endif // ENTITY_H