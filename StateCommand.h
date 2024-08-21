//------------------------------------------------------------------------------
// StateCommand.h

#ifndef TATECOMMAND_H
#define STATECOMMAND_H

#include "Command.h"

namespace Sep
{
  class StateCommand : public Command
  {
    public:
      //------------------------------------------------------------------------
      // Const char variables
      //
      static const char STATE_COMMAND[];

      //------------------------------------------------------------------------
      // Constructor of StateCommand
      //
      StateCommand();

      //------------------------------------------------------------------------
      // Executes Method
      // Executes the StateCommand Command
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

#endif STATECOMMAND_H
