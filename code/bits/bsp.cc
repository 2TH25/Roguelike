// code basique aidé du github de gf, intitulé : "DungeonGenerator_BinarySpacePartitioning.cc"

#include "bsp.h"
#include <gf/Rect.h>
#include <gf/Random.h>
#include <cassert>
#include "GameData.h"

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



    bool BSPTree::split(gf::Random& random) { // méthode split
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
        // on ne split pas si la taille max dispo pour un split est trop petite

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

    void BSPTree::recursiveSplit(gf::Random& random) { // on va appeler récursivement la méthode split 
        assert(!left && !right);
        assert(leafSizeMinimum <= leafSizeMaximum);

        if(space.getWidth() > leafSizeMaximum || space.getHeight() > leafSizeMaximum || random.computeBernoulli(0.2)) { // si notre rectangle que l'on visite est trop grand alors on le split
            if(split(random)) { // appel du split
                assert(left);
                left->recursiveSplit(random);
                assert(right);
                right->recursiveSplit(random);
                // et ensuite appel du split sur les enfants
            }
        }
    }

    void BSPTree::getRooms(std::vector<BSPTree*>& rooms) {
        if (left || right) {
            if (left) left->getRooms(rooms);
            if (right) right->getRooms(rooms);
        } else {
            rooms.push_back(this);
        }
    }

    void BSPTree::createRooms(gf::Random& random) {
        assert(RoomMinSize <= RoomMaxSize);

        if(left || right) {
            assert(left && right);

            //appel récursif sur les enfants
            left->createRooms(random);
            right->createRooms(random);

            if (random.computeBernoulli(0.5)) { //on garde l'information d'une des salles de nos 2 enfants
                room = left->room;
            } else {
                room = right->room;
            }
        } else { //sinon, vu qu'on est sur une feuille
            assert(RoomMinSize <= std::min(RoomMaxSize, space.getWidth() - 1));
            assert(RoomMinSize <= std::min(RoomMaxSize, space.getHeight() - 1)); // on vérifie que les dimensions soient bonnes

            gf::Vector2i position, size; // on créer un vecteur
            size.width = random.computeUniformInteger(RoomMinSize,std::min(RoomMaxSize, space.getWidth() - 1));
            size.height = random.computeUniformInteger(RoomMinSize,std::min(RoomMaxSize, space.getHeight() - 1));
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

    Dungeon BSP::generate(gf::Random& random) {
    switch (getPhase()) {
      case Phase::Start:
        m_savedRandom = random;
        // fallthrough
      case Phase::Iterate:
        m_random = m_savedRandom;
        generateRooms();
        // fallthrough
      case Phase::Finish:
        random = m_random;
        break;
    }

    setPhase(Phase::Finish);
    return m_dungeon;
  }

  void BSP::generateRooms() {
    m_dungeon = Dungeon(MapSize, TileType::Wall);

    m_root.space = gf::RectI::fromPositionSize({ 0, 0 }, MapSize);
    m_root.left = nullptr;
    m_root.right = nullptr;

    m_root.recursiveSplit(m_random);
    m_root.createRooms(m_random);

    std::vector<BSPTree*> leaves;
    m_root.getRooms(leaves); // On récupère toutes les salles finales

    if (leaves.size() >= 2) {
        std::shuffle(leaves.begin(), leaves.end(), m_random.getEngine());

        leaves[0]->type = RoomType::Start;
        leaves[1]->type = RoomType::End;

        for (size_t i = 2; i < leaves.size(); ++i) {
            float roll = m_random.computeUniformFloat(0.0f, 1.0f);
            
            if (roll < 0.20f) { 
                leaves[i]->type = RoomType::Healing;
            } else if (roll < 0.35f) { 
                leaves[i]->type = RoomType::Chest;
            } else {
                leaves[i]->type = RoomType::Normal;
            }
        }
    }

    walkTree(m_root);
  }

  void BSP::walkTree(const BSPTree& tree) {
    if (tree.left || tree.right) {
      assert(tree.left && tree.right);
      walkTree(*tree.left);
      walkTree(*tree.right);

      auto leftRoom = tree.left->room.getCenter();
      auto rightRoom = tree.right->room.getCenter();

      if (m_random.computeBernoulli(0.5)) {
        createHorizontalTunnel(rightRoom.x, leftRoom.x, rightRoom.y);
        createVerticalTunnel(leftRoom.x, leftRoom.y, rightRoom.y);
      } else {
        createVerticalTunnel(rightRoom.x, leftRoom.y, rightRoom.y);
        createHorizontalTunnel(rightRoom.x, leftRoom.x, leftRoom.y);
      }
    } else {
      createRoom(tree.room);
    }
  }

  void BSP::createRoom(const gf::RectI& room) {
    for (int x = room.min.x + 1; x < room.max.x; ++x) {
      for (int y = room.min.y + 1; y < room.max.y; ++y) {
        m_dungeon.setTile({ x, y }, TileType::Floor);
      }
    }
  }

  void BSP::createHorizontalTunnel(int x1, int x2, int y) {
    if (x2 < x1) {
      std::swap(x1, x2);
    }

    for (int x = x1; x <= x2; ++x) {
      m_dungeon.setTile({ x, y }, TileType::Floor);
    }
  }

  void BSP::createVerticalTunnel(int x, int y1, int y2) {
    if (y2 < y1) {
      std::swap(y1, y2);
    }

    for (int y = y1; y <= y2; ++y) {
      m_dungeon.setTile({ x, y }, TileType::Floor);
    }
  }


    

}