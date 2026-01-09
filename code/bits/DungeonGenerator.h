// #ifndef DUNGEON_GENERATOR_H
// #define DUNGEON_GENERATOR_H

// #include <gf/Array2D.h>
// #include <gf/Random.h>
// #include <gf/Vector.h>
// #include "Tile.h"
// #include "bsp.h"

// namespace rCMI {

//   using Dungeon = gf::Array2D<TileType, int>;

//   class DungeonGenerator {
//   public:
//     enum class Phase {
//       Start,
//       Iterate,
//       Finish,
//     };

//     DungeonGenerator();
//     virtual ~DungeonGenerator();

//     Phase getPhase() const {
//       return m_phase;
//     }

//     void setPhase(Phase phase) {
//       m_phase = phase;
//     }

//     virtual Dungeon generate(gf::Random& random) = 0;

//   private:
//     Phase m_phase;
//   };

// }


// #endif