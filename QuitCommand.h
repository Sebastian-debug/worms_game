//------------------------------------------------------------------------------
// QuitCommand.h

#ifndef QUITCOMMAND_H
#define QUITCOMMAND_H

#include "Command.h"

namespace Sep
{
  class QuitCommand : public Command
  {
    public:
      //------------------------------------------------------------------------
      // Const char variables
      //
      static const char QUIT_COMMAND[];

      //------------------------------------------------------------------------
      // Const int variables
      //
      static const int QUIT_VALUE = -10;

      //------------------------------------------------------------------------
      // Constructor of QuitCommand
      //
      QuitCommand();

      //------------------------------------------------------------------------
      // Executes Method
      // Executes the QuitCommand Command
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

#endif QUITCOMMAND_H
