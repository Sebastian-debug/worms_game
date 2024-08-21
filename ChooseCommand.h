//------------------------------------------------------------------------------
// ChooseCommand.h

#ifndef CHOOSECOMMAND_H
#define CHOOSECOMMAND_H

#include "Command.h"

namespace Sep
{
  class ChooseCommand : public Command
  {
    public:
      //------------------------------------------------------------------------
      // Const char variables
      //
      static const char CHOOSE_COMMAND[];

      //------------------------------------------------------------------------
      // Constructor of ChooseCommand
      //
      ChooseCommand();

      //------------------------------------------------------------------------
      // Execute Method
      // Executes the Choose Command
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

#endif //CHOOSECOMMAND_H
