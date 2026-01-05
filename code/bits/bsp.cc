// code basique aidé du github de gf, intitulé : "DungeonGenerator_BinarySpacePartitioning.cc"

#include "bsp.h"
#include <gf/Rect.h>



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

    int max = splitHorizontally ? space.getHeight() : space.getWidth();
    //dimension maximale du split (en fonction de si on split horizontalement ou pas)


    if (max <= 2 * leafSizeMinimum) {
      return false;
    }
    // on ne split pas si la taille max dispo pour un split es trop petit

    assert(leafSizeMinimum <= max - leafSizeMinimum);
    //on vérifie que le max soit suffisamment grand

    int split = random.computeUniformInteger(leafSizeMinimum, max - leafSizeMinimum);
    // détermination aléatoire de la position de l'endroit d'où on splitera

    if (splitHorizontally) { // si le rectangle a été divisé horizontalement
        left = std::make_unique<BSPTree>(gf::RectI::fromPositionSize(space.min, { space.getWidth(), split})); // la cellule divisée de gauche est un bspTree, avec une racine : un RectI, avec comme position la position minimum du rectangle divisé, et la taille, la largeur du rectangle jusqu'à l'endroit où l'on coupe
        right = std::make_unique<BSPTree>(gf::RectI::fromPositionSize({space.min.x, space.min.y + split}, { space.getWidth(), space.getHeight() - split})); // celle de droite
    } else { // sinon
        left  = std::make_unique<BSPTree>(gf::RectI::fromPositionSize(space.min, { split, space.getHeight()}));  
        right = std::make_unique<BSPTree>(gf::RectI::fromPositionSize({space.min.x + split, space.min.y}, { space.getWidth() - split, space.getHeight()}));
    }

    return true;
}

