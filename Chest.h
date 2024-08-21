//------------------------------------------------------------------------------
// Chest.h
//

#ifndef CHEST_H
#define CHEST_H

#include "Field.h"

namespace Sep
{
  class Random;

  class Chest : public Sep::Field
  {
    public:
      //------------------------------------------------------------------------
      // Constructor of Chest
      //
      Chest(Sep::Random &random_object, int board_width);

      //------------------------------------------------------------------------
      // Getter Methods
      //
      int getRow();

      Weapon getLoot();

    private:
      //------------------------------------------------------------------------
      // Const int variables
      //
      static const int NUMBER_OF_WEAPONS = 5;

      //------------------------------------------------------------------------
      // Coordinate where the chest is
      //
      int row_;

      //------------------------------------------------------------------------
      // Loot in the chest
      //
      Weapon loot_;

  };

}
#endif CHEST_H
