//------------------------------------------------------------------------------
// QuitCommand.cpp

#include <iostream>
#include "QuitCommand.h"
#include "Game.h"

//------------------------------------------------------------------------------
// Recurring printed lines and directions
//
const char Sep::QuitCommand::QUIT_COMMAND[] = "quit";

//------------------------------------------------------------------------------
Sep::QuitCommand::QuitCommand() : Command(QUIT_COMMAND)
{
}

//------------------------------------------------------------------------------
int Sep::QuitCommand::execute(Sep::Game &game, std::vector<std::string> &params, int id)
{
  if(params.size() > 1) {
    std::cout << Game::WRONG_PARA_COUNT;
    return Game::BAD_PARAMETERS;
  }
  return QUIT_VALUE;
}
