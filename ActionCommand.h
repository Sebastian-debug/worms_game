//------------------------------------------------------------------------------
// ActionCommand.h

#ifndef ACTIONCOMMAND_H
#define ACTIONCOMMAND_H

#include <set>
#include "Command.h"

namespace Sep
{
  class ActionCommand : public Command
  {
    public:
      //------------------------------------------------------------------------
      // Const char/string variables
      //
      static const char ACTION_COMMAND[];
      static const char IDLE[];
      static std::set<std::string> DIRECTIONS;

      //------------------------------------------------------------------------
      // Const int variables
      //
      static const int BAD_ACTION = -1;

      //------------------------------------------------------------------------
      // Constructor of ActionCommand
      //
      ActionCommand();

      //------------------------------------------------------------------------
      // Execute Method
      // Executes the Action Command
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

#endif //ACTIONCOMMAND_H
