
// code basique aidé du github de gf, intitulé : "DungeonGenerator_BinarySpacePartitioning.h"

struct BSPTree{
    std::unique_ptr<ArbreBSP> left;
    std::unique_ptr<ArbreBSP> right; // les deux fils d'un noeud splité

    gf::RectI space; // l'espace correspondant au rectangle
    gf::RectI room; // la salle dans le rectangle


    // voici la strucuture RectI pour mieux se la représenter :
    // struct RectI {
    //     gf::Vector2i position; // x, y
    //     gf::Vector2i size;     // w, h
    // };

    BSPTree(gf::RectI initialSpace);

    bool split(gf::Random& random, int leafSizeMinimum); // méthode pour split
    void recursiveSplit(gf::Random& random, int leafSizeMinimum, int leafSizeMaximum); // split récursif
    void createRooms(gf::Random& random, int roomSizeMinimum, int roomSizeMaximum); // création des salles

}