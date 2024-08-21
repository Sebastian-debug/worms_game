//------------------------------------------------------------------------------
// StateCommand.cpp


#include <iostream>
#include "StateCommand.h"
#include "Game.h"

//------------------------------------------------------------------------------
// Recurring printed lines and directions
//
const char Sep::StateCommand::STATE_COMMAND[] = "state";

//------------------------------------------------------------------------------
Sep::StateCommand::StateCommand() : Command(STATE_COMMAND)
{
}

//------------------------------------------------------------------------------
int Sep::StateCommand::execute(Sep::Game &game,
  std::vector<std::string> &params, int id)
{
  if (params.size() > 1)
  {
    std::cout << Game::WRONG_PARA_COUNT;
    return Game::BAD_PARAMETERS;
  }
  game.print_state();

  return Game::SUCCESS_CODE;
}
