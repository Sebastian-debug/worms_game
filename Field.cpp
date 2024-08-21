//------------------------------------------------------------------------------
// Field.cpp
//

#include <stdexcept>
#include "Field.h"
#include "Game.h"

using Sep::Field;

//------------------------------------------------------------------------
Field::Field(Field::FieldType type) : type_(type)
{
}

//------------------------------------------------------------------------
char Field::getCharacter()
{
  return static_cast<char>(type_);
}

//------------------------------------------------------------------------
void Field::setType(Field::FieldType type)
{
  type_ = type;
}

//------------------------------------------------------------------------
Field::FieldType Field::getType()
{
  return type_;
}

//------------------------------------------------------------------------
Field::~Field() noexcept
{
}

//------------------------------------------------------------------------
std::string Sep::Field::getWeaponName(Field::Weapon weapon)
{
  switch (weapon)
  {
    case Weapon::GUN:
      return "gun";
    case Weapon::BAZOOKA:
      return "bazooka";
    case Weapon::TELEPORT:
      return "teleport";
    case Weapon::BLOWTORCH:
      return "blowtorch";
    case Weapon::MELEE:
      return "melee";
    case Weapon::AIRSTRIKE:
      return "airstrike";
    default:
      return nullptr;
  }
}

//------------------------------------------------------------------------
Field::Weapon Sep::Field::getWeaponFromName(std::string name)
{
  for (int index = static_cast<int>(Weapon::GUN); index
    <= static_cast<int>(Weapon::AIRSTRIKE); index++)
  {
    if (name == getWeaponName(static_cast<Weapon>(index)))
    {
      return static_cast<Weapon>(index);
    }
  }

  throw std::invalid_argument(Game::WRONG_PARAMS);
}

//------------------------------------------------------------------------
std::string Sep::Field::getFieldName(Field::FieldType field_type)
{
  switch (field_type)
  {
    case FieldType::AIR:
      return "Air";
    case FieldType::EARTH:
      return "Earth";
    case FieldType::WATER:
      return "Water";
    case FieldType::WORM:
      return "Worm";
    case FieldType::CHEST:
      return "Chest";
    default:
      return nullptr;
  }
}

//------------------------------------------------------------------------
int Sep::Field::getWeaponDamage(Field::Weapon weapon)
{
  switch (weapon)
  {
    case Weapon::GUN:
      return 25;
    case Weapon::BAZOOKA:
      return 40;
    case Weapon::TELEPORT:
      return -1;
    case Weapon::BLOWTORCH:
      return 35;
    case Weapon::MELEE:
      return 50;
    case Weapon::AIRSTRIKE:
      return 20;
    default:
      return -1;
  }
}
