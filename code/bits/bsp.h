#ifndef BSP_H
#define BSP_H

// code basique aidé du github de gf, intitulé : "DungeonGenerator_BinarySpacePartitioning.h"

#include <gf/Rect.h>
#include <memory>
#include <gf/Random.h>

#include "DungeonGenerator.h"
#include "Tile.h"

namespace rCMI
{
  enum class RoomType
  {
    Normal,
    Start,
    End,
    Healing,
    Chest
  };

  struct BSPTree
  {
    std::unique_ptr<BSPTree> left;
    std::unique_ptr<BSPTree> right; // les deux fils d'un noeud splité

    gf::RectI space; // l'espace correspondant au rectangle
    gf::RectI room;  // la salle dans le rectangle
    RoomType type = RoomType::Normal;

    // voici la strucuture RectI pour mieux se la représenter :
    // struct RectI {
    //     gf::Vector2i position; // x, y
    //     gf::Vector2i size;     // w, h
    // };

    BSPTree(gf::RectI initialSpace);

    int getWidth() const;
    int getHeight() const;

    bool split(gf::Random &random);          // méthode pour split
    void recursiveSplit(gf::Random &random); // split récursif
    void createRooms(gf::Random &random);    // création des salles
    void getRooms(std::vector<BSPTree *> &rooms);
  };

  class BSP : public DungeonGenerator
  {
  public:
    BSP();

    int leafSizeMinimum = 10;
    int leafSizeMaximum = 24;

    Dungeon generate(gf::Random &random, gf::Vector2i sizeMap) override;
    BSPTree &getRoot() { return m_root; }

  private:
    gf::Random m_savedRandom;
    gf::Random m_random;
    BSPTree m_root;
    Dungeon m_dungeon;

    void generateRooms(gf::Vector2i sizeMap);
    void walkTree(const BSPTree &tree);
    void createRoom(const gf::RectI &room);
    void createHorizontalTunnel(int x1, int x2, int y);
    void createVerticalTunnel(int x, int y1, int y2);
  };
}

#endif
