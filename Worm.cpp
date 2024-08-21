//------------------------------------------------------------------------------
// Worm.cpp

#include "Worm.h"
#include "Field.h"
#include <sstream>
#include <limits>
#include <iostream>
#include <utility>

//------------------------------------------------------------------------------
Sep::Worm::Worm(int id, std::string name) : Field(Field::FieldType::WORM),
  worm_name_(name), hp_(100), worm_id_(id)
{
  team_ = (id < 4) ? TEAM_A : TEAM_B;
  weapons_ = {1, 1, 1, 1, 1};
  equipped_ = Field::Weapon::GUN;
}

//------------------------------------------------------------------------------
const char Sep::Worm::getTeamChar()
{
  return this->team_;
}

//------------------------------------------------------------------------------
int Sep::Worm::getID()
{
  return this->worm_id_;
}

//------------------------------------------------------------------------------
std::string Sep::Worm::getName()
{
  return this->worm_name_;
}

//------------------------------------------------------------------------------
bool Sep::Worm::damage(int damage, bool display_damage)
{
  hp_ -= damage;
  if (display_damage && hp_ > 0)
  {
    std::cout << worm_name_ << " (" << worm_id_ << ") took " << damage
      << "hp damage\n";
  }
  return hp_ < 1;
}

//------------------------------------------------------------------------------
int Sep::Worm::getHP()
{
  return hp_;
}

//------------------------------------------------------------------------------
void Sep::Worm::addAmmunition(Weapon weapon)
{
  if (weapon == Weapon::GUN)
  {
    return;
  }
  weapons_[static_cast<int>(weapon)]++;
  std::cout << worm_name_ << " (" << worm_id_ << ") picked up 1 of "
    << getWeaponName(weapon) << "\n";
}

//------------------------------------------------------------------------------
void Sep::Worm::equipWeapon(Sep::Field::Weapon weapon)
{
  equipped_ = weapon;
}

//------------------------------------------------------------------------------
Sep::Field::Weapon Sep::Worm::getEquipped()
{
  return equipped_;
}

//------------------------------------------------------------------------------
std::string Sep::Worm::getWeaponList()
{
  std::stringstream buffer;
  buffer << "weapons of current worm:\n";
  for (int i = -1; i < static_cast<int>(weapons_.size()); i++)
  {
    if (i != -1 && weapons_[i] == 0)
    {
      continue;
    }
    buffer << "  " << Field::getWeaponName(static_cast<Weapon>(i))
      << " (ammunition: " << ((i == -1) ? "inf" : std::to_string(weapons_[i]))
      << ")" << ((i == static_cast<int>(equipped_)) ? " *\n" : "\n");
  }
  return buffer.str();
}

//------------------------------------------------------------------------------
int Sep::Worm::getAmmunition(Sep::Field::Weapon weapon)
{
  return (weapon == Weapon::GUN) ? std::numeric_limits<int>::max()
    : weapons_[static_cast<int>(weapon)];
}

//------------------------------------------------------------------------------
void Sep::Worm::useAmmunition(Sep::Field::Weapon weapon)
{
  if (weapon == Weapon::GUN)
  {
    return;
  }
  weapons_[static_cast<int>(weapon)]--;
}




