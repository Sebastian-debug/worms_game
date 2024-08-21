//------------------------------------------------------------------------------
// Chest.cpp

#include "Chest.h"
#include "Random.h"

//------------------------------------------------------------------------------
Sep::Chest::Chest(Sep::Random &random_object, int board_width) :
  Field(FieldType::CHEST),
  row_(random_object.getRandomInt(0, board_width - 1)),
  loot_(static_cast<Weapon>(random_object.getRandomInt(0, NUMBER_OF_WEAPONS-1)))
{
}

//------------------------------------------------------------------------------
int Sep::Chest::getRow()
{
  return row_;
}

//------------------------------------------------------------------------------
Sep::Chest::Weapon Sep::Chest::getLoot()
{
  return loot_;
}

