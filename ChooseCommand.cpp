//------------------------------------------------------------------------------
// ChooseCommand.cpp

#include <iostream>
#include "ChooseCommand.h"
#include "Game.h"
#include "Field.h"

//------------------------------------------------------------------------------
// Recurring printed lines
//
const char Sep::ChooseCommand::CHOOSE_COMMAND[] = "choose";

//------------------------------------------------------------------------------
Sep::ChooseCommand::ChooseCommand() : Command(CHOOSE_COMMAND)
{
}

//------------------------------------------------------------------------------
int Sep::ChooseCommand::execute(Sep::Game &game,
  std::vector<std::string> &params, int id)
{
  if (params.size() != 2)
  {
    std::cout << Game::WRONG_PARA_COUNT;
    return Game::BAD_PARAMETERS;
  }

  try
  {
    Field::Weapon weapon = Field::getWeaponFromName(params[1]);
    game.equipWeapon(weapon, id);
  }
  catch (const std::invalid_argument &e)
  {
    std::cout << e.what();
  }
  return Game::SUCCESS_CODE;
}
