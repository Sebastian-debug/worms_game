//------------------------------------------------------------------------------
// HelpCommand.h

#ifndef HELPCOMMAND_H
#define HELPCOMMAND_H

#include "Command.h"

namespace Sep
{
  class HelpCommand : public Command
  {
    public:
      //------------------------------------------------------------------------
      // Const char variables
      //
      static const char HELP_COMMAND[];

      //------------------------------------------------------------------------
      // Constructor of HelpCommand
      //
      HelpCommand();

      //------------------------------------------------------------------------
      // Executes Method
      // Executes the HelpCommand Command
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

#endif //HELPCOMMAND_H
