//------------------------------------------------------------------------------
// Field.h
//

#ifndef FIELD_H
#define FIELD_H

#include <string>

namespace Sep
{
  class Field
  {
    public:
      //------------------------------------------------------------------------
      // Fieldtype enum
      //
      enum class FieldType{AIR = ' ', EARTH = 'E', WATER = 'W', WORM,
        CHEST = '#'};

      //------------------------------------------------------------------------
      // Weapons enum
      //
      enum class Weapon {GUN = -1, BAZOOKA = 0, TELEPORT = 1, BLOWTORCH = 2,
        MELEE = 3, AIRSTRIKE = 4};

      //------------------------------------------------------------------------
      //Standard Constructor
      //
      Field();
      //------------------------------------------------------------------------
      // The Constructor of the Field Class
      //
      //
      Field(FieldType type);

      //------------------------------------------------------------------------
      // Getter Methods
      //
      char getCharacter();

      FieldType getType();

      static std::string getFieldName(FieldType field_type);

      static std::string getWeaponName(Weapon weapon);

      static Weapon getWeaponFromName(std::string name);

      static int getWeaponDamage(Weapon weapon);

      //------------------------------------------------------------------------
      // Setter Methods
      //
      void setType(FieldType type);

      //------------------------------------------------------------------------
      // Virtual Destructor that is not allowed to throw execptions
      //
      virtual ~Field() noexcept;

    private:
      //------------------------------------------------------------------------
      // Field type
      //
      FieldType type_;

  };
}
#endif //FIELD_H