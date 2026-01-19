#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <gf/Color.h>
#include <gf/Vector.h>

namespace rCMI
{
  class Existence
  {
  public:
    Existence() = default;

    Existence(gf::Vector2i pos, char16_t ch, gf::Color4f col, std::string n, bool blocks)
        : position(pos),
          character(ch),
          color(col),
          name(std::move(n)),
          blocks_movement(blocks)
    {
    }

    const gf::Vector2i &getPosition() const { return position; }
    char16_t getCharacter() const { return character; }
    const gf::Color4f &getColor() const { return color; }
    const std::string &getName() const { return name; }
    bool getBlocksMovement() const { return blocks_movement; }
    void setPosition(gf::Vector2i pos) { position = pos; }
    void setCharacter(char16_t ch) { character = ch; }
    void setColor(const gf::Color4f &col) { color = col; }
    void setName(const std::string &n) { name = n; }
    void setBlocksMovement(bool blocks) { blocks_movement = blocks; }

  private:
    gf::Vector2i position = {0, 0};
    char16_t character = u' ';
    gf::Color4f color = gf::Color::White;
    std::string name = "nothing";
    bool blocks_movement = false;
  };
}

#endif // ENTITY_H