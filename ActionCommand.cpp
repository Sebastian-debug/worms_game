//------------------------------------------------------------------------------
// ActionCommand.cpp

#include <iostream>
#include <algorithm>
#include "ActionCommand.h"
#include "Game.h"

//------------------------------------------------------------------------------
// Recurring printed lines and directions
//
const char Sep::ActionCommand::ACTION_COMMAND[] = "action";
const char Sep::ActionCommand::IDLE[] = "idle";
std::set<std::string> Sep::ActionCommand::DIRECTIONS = {"l", "r", "d", "u",
  "ld", "rd", "lu", "ru"};

//------------------------------------------------------------------------------
Sep::ActionCommand::ActionCommand() : Command(ACTION_COMMAND)
{
}

//------------------------------------------------------------------------------
int Sep::ActionCommand::execute(Sep::Game &game,
  std::vector<std::string> &params, int id)
{
  if (params.size() > 3)
  {
    std::cout << Game::WRONG_PARA_COUNT;;
    return BAD_ACTION;
  }

  if (params.size() == 1)
  {
    return game.melee_attack(id);
  }

  if (params.size() == 2)
  {
    if (params[1] == IDLE)
    {
      return Game::SUCCESS_CODE;
    }
    if (DIRECTIONS.find(params[1]) != DIRECTIONS.end())
    {
      return game.shoot_weapon(params[1], id);
    }
    try
    {
      int col = std::stoi(params[1]);
      return game.airstrike(col, id);
    }
    catch (...)
    {
      std::cout << Game::WRONG_PARAMS;
      return BAD_ACTION;
    }
  }

  if (params.size() == 3)
  {
    try
    {
      int row = std::stoi(params[1]);
      int col = std::stoi(params[2]);
      return game.teleport(row, col, id);
    }
    catch (...)
    {
      std::cout << Game::WRONG_PARAMS;
      return BAD_ACTION;
    }
  }
  return Game::SUCCESS_CODE;
}
