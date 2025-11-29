// code basique aidé du github de gf, intitulé : "DungeonGenerator_BinarySpacePartitioning.cc"

#include "bsp.h"



BSPTree::BSPTree(gf::RectI initialSpace) { // constructeur du BSPTree
    left = nullptr;
    right = nullptr;
    space = initialSpace;
}

//getters de largeur et hauteur pour BSPTree
int BSPTree::getWidth() const { 
    return space.getSize().x;
}

int BSPTree::getHeight() const {
    return space.getSize().y;
}



bool BSPTree::split(gf::Random& random, int leafSizeMinimum) { // méthode split
    if(left||right) { // si le noeud en question a déjà un fils gauche ou droite, alors on ne peut pas le split
        return false; // on renvoie faux
    }

    bool splitHorizontally = random.computeBernoulli(0.5f);
    // renvoie true ou false, 50% du temps l'un et 50% l'autre
    //selon la loi de Bernouilli (true = p et false = 1 - p)

    if(space.getWidth() >= 1.25 * space.getHeight()) {
        splitHorizontally = false; // on force à faux la division horizontale 
        // si le rectangle est déjà trop large
        
    }
    else if (space.getHeight() >= 1.25 * space.getWidth()){
        splitHorizontally = true;
        //sinon si le rectangle est déjà trop haut, alors on s'assure de 
        // le diviser horizontalement

    }


}