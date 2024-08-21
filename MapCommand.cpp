//------------------------------------------------------------------------------
// MapCommand.cpp

#include <iostream>
#include "MapCommand.h"
#include "Game.h"

//------------------------------------------------------------------------------
// Recurring printed lines and directions
//
const char Sep::MapCommand::MAP_COMMAND[] = "map";

//------------------------------------------------------------------------------
Sep::MapCommand::MapCommand() : Command(MAP_COMMAND)
{
}

//------------------------------------------------------------------------------
int Sep::MapCommand::execute(Sep::Game &game, std::vector<std::string> &params,
  int id)
{
  if (params.size() > 1)
  {
    std::cout << Game::WRONG_PARA_COUNT;
    return Game::BAD_PARAMETERS;
  }

  game.printMap();

  return Game::SUCCESS_CODE;
}
