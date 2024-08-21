//------------------------------------------------------------------------------
// MapCommand.h

#ifndef MAPCOMMAND_H
#define MAPCOMMAND_H

#include "Command.h"

namespace Sep
{
  class MapCommand : public Command
  {
    public:
      //------------------------------------------------------------------------
      // Const char variables
      //
      static const char MAP_COMMAND[];

      //------------------------------------------------------------------------
      // Constructor of MapCommand
      //
      MapCommand();

      //------------------------------------------------------------------------
      // Executes Method
      // Executes the MapCommand Command
      // @param game Instance of game
      // @param params Additional parameter of the command
      // @param id Id of the current worm
      // return Returns zero
      //
      int execute(Game &game, std::vector<std::string> &params,
        int id) override;

    private:
  };
}

#endif //MAPCOMMAND_H
