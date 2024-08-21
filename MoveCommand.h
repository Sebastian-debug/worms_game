//------------------------------------------------------------------------------
// MoveCommand.h

#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include "Command.h"

namespace Sep
{
  class MoveCommand : public Command
  {
    public:
      //------------------------------------------------------------------------
      // Const char variables
      //
      static const char MOVE_COMMAND[];

      //------------------------------------------------------------------------
      // Const int variables
      //
      static const int DIED_AFTER_MOVING = -2;
      static const int INVALID_COMMAND = -3;

      //------------------------------------------------------------------------
      // Constructor of MoveCommand
      //
      MoveCommand();

      //------------------------------------------------------------------------
      // Executes Method
      // Executes the MoveCommand Command
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

#endif //MOVECOMMAND_H
