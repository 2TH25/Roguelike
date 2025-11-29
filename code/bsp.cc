// code basique aidé du github de gf, intitulé : "DungeonGenerator_BinarySpacePartitioning.cc"

#include "bsp.h"


bool BSPTree::split(gf::Random& random, int leafSizeMinimum) { // cas de base de la méthode split
    if(left||right) { // si le noeud en question a déjà un fils gauche ou droite, alors on ne peut pas le split
        return false; // on renvoie faux
    }
}