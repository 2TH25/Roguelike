// code basique aidé du github de gf, intitulé : "DungeonGenerator_BinarySpacePartitioning.cc"

#include "bsp.h"
#include <gf/Rect.h>
#include <gf/Random.h>
#include <cassert>

namespace rCMI {
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

    void BSPTree::recursiveSplit(gf::Random& random, int leafSizeMinimum, int leafSizeMaximum) { // on va appeler récursivement la méthode split 
        assert(!left && !right);
        assert(leafSizeMinimum <= leafSizeMaximum);

        if(space.getWidth() > leafSizeMaximum || space.getHeight() > leafSizeMaximum || random.computeBernoulli(0.2)) { // si notre rectangle que l'on visite est trop grand alors on le split
            if(split(random, leafSizeMinimum)) { // appel du split
                assert(left);
                left->recursiveSplit(random,leafSizeMinimum,leafSizeMaximum);
                assert(right);
                right->recursiveSplit(random,leafSizeMinimum,leafSizeMaximum);
                // et ensuite appel du split sur les enfants
            }
        }
    }

    void BSPTree::createRooms(gf::Random& random, int roomSizeMinimum, int roomSizeMaximum) {
        assert(roomSizeMinimum <= roomSizeMaximum);

        if(left || right) {
            assert(left && right);

            //appel récursif sur les enfants
            left->createRooms(random,roomSizeMinimum,roomSizeMaximum);
            right->createRooms(random,roomSizeMinimum,roomSizeMaximum);

            if (random.computeBernoulli(0.5)) { //on garde l'information d'une des salles de nos 2 enfants
                room = left->room;
            } else {
                room = right->room;
            }
        } else { //sinon, vue qu'on est sur une feuille
            assert(roomSizeMinimum <= std::min(roomSizeMaximum, space.getWidth() - 1));
            assert(roomSizeMinimum <= std::min(roomSizeMaximum, space.getHeight() - 1)); // on vérifie que les dimensions soient bonnes

            gf::Vector2i position, size; // pn créer un vecteur
            size.width = random.computeUniformInteger(roomSizeMinimum,std::min(roomSizeMaximum, space.getWidth() - 1));
            size.height = random.computeUniformInteger(roomSizeMinimum,std::min(roomSizeMaximum, space.getHeight() - 1));
            position.x = random.computeUniformInteger(0,space.getWidth() - size.width - 1);
            position.y = random.computeUniformInteger(0,space.getHeight() - size.height - 1); // de manière random on définit la taille et la position de la salle dans l'espace qu'on lui décrit comme disponible
            position += space.getPosition(); // on oublie pas de prendre en compte tout le plateau pour définir la position pas de façon absolue, mais relative au plateau

            room = gf::RectI::fromPositionSize(position,size); // enfin, on créer la salle
        }
    }


    BSP::BSP()
    : m_root(gf::RectI::fromPositionSize({ 0, 0 }, { 1, 1 }))
    {
    }


    

}