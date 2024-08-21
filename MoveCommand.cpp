//------------------------------------------------------------------------------
// MoveCommand.cpp

#include "MoveCommand.h"
#include "Game.h"
#include <iostream>
#include <vector>

//------------------------------------------------------------------------------
// Recurring printed lines and directions
//
const char Sep::MoveCommand::MOVE_COMMAND[] = "move";

//------------------------------------------------------------------------------
Sep::MoveCommand::MoveCommand() : Command(MOVE_COMMAND)
{
}

//------------------------------------------------------------------------------
int Sep::MoveCommand::execute
  (Sep::Game &game, std::vector<std::string> &params, int id)
{
  if (params.size() != 3)
  {
    std::cout << Game::WRONG_PARA_COUNT;
    return INVALID_COMMAND;
  }
  if (params[1] != "l" && params[1] != "r")
  {
    std::cout << Game::WRONG_PARAMS;
    return INVALID_COMMAND;
  }
  try
  {
    int steps = std::stoi(params[2]);
    if (steps > 3 || steps < 0)
    {
      std::cout << Game::WRONG_PARAMS;
      return INVALID_COMMAND;
    }
    if (params[1] == "l")
    {
      steps = (-1) * steps;
    }
    return game.move(id, steps);
  }
  catch (...)
  {
    std::cout << Game::WRONG_PARAMS;
    return INVALID_COMMAND;
  }
}
