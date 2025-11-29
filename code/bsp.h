
// code basique aidé du github de gf, intitulé : "DungeonGenerator_BinarySpacePartitioning.h"

struct BSPTree{
    std::unique_ptr<ArbreBSP> left;
    std::unique_ptr<ArbreBSP> right; // les deux fils d'un noeud splité

    gf::RectI space; // l'espace correspondant au rectangle
    gf::RectI room; // la salle dans le rectangle

    BSPTree(gf::RectI initialSpace);

    bool split(gf::Random& random, int leafSizeMinimum); // cas de base de la méthode pour split
    void recursiveSplit(gf::Random& random, int leafSizeMinimum, int leafSizeMaximum); // split récursif
    void createRooms(gf::Random& random, int roomSizeMinimum, int roomSizeMaximum); // création des salles

}