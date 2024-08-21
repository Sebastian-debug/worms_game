//------------------------------------------------------------------------------
// main.cpp

#include "Game.h"
#include "Field.h"
#include "Command.h"
#include "Random.h"
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>

//------------------------------------------------------------------------------
// Recurring printed lines and return values
//
namespace Sep
{
  const int RETURN_OK = 0;
  const int RETURN_ERROR_BAD_CONFIG = -1;
  const int RETURN_ERROR_OUT_OF_MEMORY = -2;
  const int RETURN_ERROR_ARGUMENT = -3;
  const std::string ERROR_WRONG_ARGUMENTS =
    "[ERROR] usage: ./sworms <config-file>\n";
  const std::string ERROR_OUT_OF_MEMORY = "[ERROR] memory error!\n";
}

using std::vector;

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::cout << Sep::ERROR_WRONG_ARGUMENTS << '\n';
    return Sep::RETURN_ERROR_ARGUMENT;
  }

  try
  {
    Sep::Game game;
    Sep::Random random_object;
    if (game.loadConfig(std::string(argv[1])))
    {
      return Sep::RETURN_ERROR_BAD_CONFIG;
    }
    game.spawnWorms(random_object);
    game.printMap();
    game.mainLoop(random_object);

    return Sep::RETURN_OK;
  }
  catch(const std::bad_alloc& ex)
  {
    std::cout << Sep::ERROR_OUT_OF_MEMORY;
    return Sep::RETURN_ERROR_OUT_OF_MEMORY;
  }
}
