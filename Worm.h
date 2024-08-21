//------------------------------------------------------------------------------
// Worm.h


#ifndef WORM_H
#define WORM_H

#include <string>
#include <vector>
#include "Field.h"

namespace Sep
{
  class Chest;

  class Worm : public Sep::Field
  {
    public:
      //------------------------------------------------------------------------
      // Team variables
      //
      static const char TEAM_A = '~';
      static const char TEAM_B = '*';

      //------------------------------------------------------------------------
      // Constructor of worm
      //
      Worm(int id, std::string name);

      //------------------------------------------------------------------------
      // Getter Methods
      //
      const char getTeamChar();
      int getID();
      int getHP();
      std::string getName();
      Weapon getEquipped();
      int getAmmunition(Weapon weapon);
      std::string getWeaponList();

      //------------------------------------------------------------------------
      // Damage Method
      // Reduces the hp and prints displays how much damage was taken
      // @param damage The amount of damage
      // @param display_damage If the display of the damage value is necessary
      // return Returns true if worm is dead or false if not
      //
      bool damage(int damage, bool display_damage);

      //------------------------------------------------------------------------
      // Add Ammunition Method
      // Adds ammunition to the inventory
      // @param weapon To which weapon the collected ammunition belongs
      //
      void addAmmunition(Weapon weapon);

      //------------------------------------------------------------------------
      // Using Ammunition Method
      // Use the ammunition while shooting/doing something (except gun)
      // @param weapon Which weapon is used
      //
      void useAmmunition(Weapon weapon);

      //------------------------------------------------------------------------
      // Equip Weapon Method
      // Equips the desired weapon
      // @param weapon Which weapon should be equipped
      //
      void equipWeapon(Weapon weapon);

    private:
      //------------------------------------------------------------------------
      // Worm name
      //
      std::string worm_name_;

      //------------------------------------------------------------------------
      // Worm HP
      //
      int hp_;

      //------------------------------------------------------------------------
      // Worm ID
      //
      int worm_id_;

      //------------------------------------------------------------------------
      // Currently equipped weapon
      //
      Weapon equipped_;

      //------------------------------------------------------------------------
      // Team ~ or Team *
      //
      char team_;

      //------------------------------------------------------------------------
      // Weapons inventory
      //
      std::vector<int> weapons_;
  };

}

#endif //WORM_H
