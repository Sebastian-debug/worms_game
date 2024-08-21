//------------------------------------------------------------------------------
// HelpCommand.cpp

#include <iostream>
#include "HelpCommand.h"
#include "Game.h"

//------------------------------------------------------------------------------
// Recurring printed lines and directions
//
const char Sep::HelpCommand::HELP_COMMAND[] = "help";

//------------------------------------------------------------------------------
Sep::HelpCommand::HelpCommand() : Command(HELP_COMMAND)
{
}

//------------------------------------------------------------------------------
int Sep::HelpCommand::execute(Sep::Game &game, std::vector<std::string> &params,
  int id)
{
  if (params.size() > 1)
  {
    std::cout << Game::WRONG_PARA_COUNT;
    return Game::BAD_PARAMETERS;
  }

  std::cout << "Available Commands:\n"
               "  move [l/r] [0..3]\n"
               "    Move your worm.\n"
               "  choose [weapon]\n"
               "    Select your weapon.\n"
               "  action [l/r/d/u/ld/rd/lu/ru]\n"
               "    Shoots current weapon.\n"
               "    Activates blowtorch.\n"
               "  action\n"
               "    Activates melee attack.\n"
               "  action [row] [col]\n"
               "    Teleports to coordinate.\n"
               "  action [col]\n"
               "    Airstrike.\n"
               "  action idle\n"
               "    Do nothing.\n"
               "  quit\n"
               "    End the game.\n"
               "  help\n"
               "    Display this help.\n"
               "  state\n"
               "    Print the current game state.\n";
  return Game::SUCCESS_CODE;
}
