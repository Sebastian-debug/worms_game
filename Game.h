//------------------------------------------------------------------------------
// Game.h

#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <map>
#include <memory>
#include "Field.h"


namespace Sep
{
  using std::vector;
  using std::string;

  class Random;

  class Worm;

  class Command;

  class Game
  {
    public:

      //------------------------------------------------------------------------
      // Const int variables
      //
      static const int SUCCESS_CODE = 0;
      static const int INVALID_CONFIG_CODE = -1;
      static const int BAD_PARAMETERS = -1;

      //------------------------------------------------------------------------
      // Const char variables
      //
      static const char WRONG_PARA_COUNT[];
      static const char WRONG_PARAMS[];
      static const char INVALID_CONFIG[];
      static const char INVALID_TARGET[];
      static const char MOVE_WARNING[];
      static const char SHOT_MISSED[];
      static const char NO_AMMUNITION[];
      static const char SHOT[];
      static const char TORCH[];
      static const char ATTACK[];
      static const char UNKNOWN_COMMAND[];
      static const char ILLEGAL_COMMAND[];
      static const char TEAM_A_VICTORY[];
      static const char TEAM_B_VICTORY[];
      static const char DRAW[];

      //------------------------------------------------------------------------
      // Constructor of Game, initializes the required variables
      //
      Game();

      //------------------------------------------------------------------------
      // The Destructor of Game. Deallocate all pointers in map_
      //
      ~Game();

      //------------------------------------------------------------------------
      // Loads Config Method
      // Loads the provided config and reads in board_width_, board_height_,
      // and map_
      // @param cfg_file The path to the config to be loaded
      // @return Returns zero on success, minus one on failure
      //
      int loadConfig(string cfg_file);

      //------------------------------------------------------------------------
      // Spawn Worms Method
      // Spawns the six worms to a random location
      // if possible (considers gravity)
      // @param random_object Instance of the Class Random
      // @return Returns zero
      //
      int spawnWorms(Sep::Random &random_object);

      //------------------------------------------------------------------------
      // Print Map Method
      // Prints the map to the console
      //
      void printMap();

      //------------------------------------------------------------------------
      // Moves Worm Method
      // Moves the worm on the map, max 3 steps left or right.The worm can climb
      // on an neighbouring earth field or other worm if there is air above.
      // @param id Id of the current worm
      // @param steps the steps to be taken from the original position. Use
      //        negative numbers to move left.
      // @return Returns zero on success(and also on invalid target)and minus
      //         two in case of death after moving.
      //
      int move(int id, int steps);

      //------------------------------------------------------------------------
      // Main Loop Method
      // The main loop for the player change and command prompt.
      // @param random_object Instance of the Class Random
      // @return Returns zero on victory
      //
      int mainLoop(Sep::Random &random_object);

      //------------------------------------------------------------------------
      // Player Change Method
      // Switches the player of each team.
      // @param last_id Id of the latest worm
      // @param team Char (~ or *) of team
      // @return Returns shared pointer of next worm. If there is no worm
      // of a team alive, it returns a Nullpointer
      //
      std::shared_ptr<Worm> getNextOfTeam(int last_id, char team);

      //------------------------------------------------------------------------
      // Spawn Chest Method
      // To spawn a chest with a random weapon.
      // @param random_object Instance of the Class Random
      //
      void spawn_Chest(Sep::Random &random_object);

      //------------------------------------------------------------------------
      // Melee Attack Method
      // The worm uses the baseball bat (50 damage, 8-connected)
      // @param worm_id Id of the worm that uses the weapon
      // @param team Char (~ or *) of team
      // @return Returns zero on success and minus one if the command is invalid
      //
      int melee_attack(int worm_id);

      //------------------------------------------------------------------------
      // Teleport Method
      // The worm uses the teleport (the worm teleports to a valid position)
      // @param row The row of the desired telport position
      // @param col The column of the desired teleport position
      // @param worm_id Id of the worm that uses the weapon
      // @return Returns zero on success and minus one if the command is invalid
      //
      int teleport(int row, int col, int worm_id);

      //------------------------------------------------------------------------
      // Airstrike Method
      // The worm uses the airstrike (20 damage)
      // @param col The col where the airstrike should take place
      // @param worm_id Id of the worm that uses the weapon
      // @return Returns zero on success and minus one if the command is invalid
      //
      int airstrike(int col, int worm_id);

      //------------------------------------------------------------------------
      // Shoot Weapon Method
      // Shoots gun, bazooka or blowtorch.
      // @param direction Direction of the weapon
      // @param worm_id Id of the worm that uses the weapon
      // @return Returns zero on success and minus one if the command is invalid
      //
      int shoot_weapon(std::string direction, int worm_id);

      //------------------------------------------------------------------------
      // Equip Weapon Method
      // Equips the desired weapon.
      // @param weapon The weapon to equip
      // @param worm_id Id of the worm that uses the weapon
      //
      void equipWeapon(Field::Weapon weapon, int worm_id);

      //------------------------------------------------------------------------
      // Print State Method
      // Prints the current state (worm stats, names, position and weapons)
      //
      void print_state();

    private:
      //------------------------------------------------------------------------
      // Const int variables
      //
      static const int MIN_SIZE = 10;
      static const int MAX_SIZE = 80;
      static const int DROWNED = -1;
      static const int FELL_OUT = -2;
      static const int TOO_HIGH = -3;

      //------------------------------------------------------------------------
      // The current team
      //
      char current_team_;

      //------------------------------------------------------------------------
      // The current worm
      //
      std::shared_ptr<Worm> current_worm_;

      //------------------------------------------------------------------------
      // Enum for the cause of death
      //
      enum Cause {DROWNING, FALL_DAMAGE, FALLING, WEAPONS};

      //------------------------------------------------------------------------
      // The width of the board
      //
      int board_width_;

      //------------------------------------------------------------------------
      // The height of the board
      //
      int board_height_;

      //------------------------------------------------------------------------
      // The map, containing a field object for each field
      //
      vector<vector<std::shared_ptr<Field>>> map_;

      //------------------------------------------------------------------------
      // Map to store where to worms are placed.
      //
      std::map<int, vector<int>> worms_;

      //------------------------------------------------------------------------
      // Kill Worm Method
      // To remove ID from Map if worm dies
      // @param worm The worm that died
      // @param cause The death cause
      //
      void killWorm(std::shared_ptr<Worm> worm, Cause cause);

      //------------------------------------------------------------------------
      // Empty Line Method
      // Checks whether a line is empty (=no non-whitespace chars or Comment)
      // @param line the string literal to be checked
      // @return bool whether the line is empty
      //
      bool isLineEmpty(string line);

      //------------------------------------------------------------------------
      // Worm Fall Method
      // Handles the fall damage and cause of death.
      // @param worm The worm that falls
      // @param row The row of the worm
      // @param col The column of the worm
      // @return Returns the row of the worm, in case of drowning minus two,
      // in case of falling out of map minus one
      //
      int wormFall(std::shared_ptr<Worm> worm, int row, int col);

      //------------------------------------------------------------------------
      // Trim Method
      // Trims the whitespace characters off the end of the String
      // @param str the String to be trimmed
      // @return the trimmed string
      string trim(string &str);

      //------------------------------------------------------------------------
      // Parse Width And Height Method
      // Parses the Width and Height from a provided String. Does not check
      // whether the required prefix is present. Checks whether the values are
      // valid.
      // @param line the String literal to be parsed for the values
      // @return Returns zero on success and minus one on illegal value
      //
      int parseWidthAndHeight(string line);

      //------------------------------------------------------------------------
      // Check Victory Method
      // Checks for draw, win Player 1 (~) or win Player 2 (*)
      // @return Returns true if one of those cases occurs, otherwise false
      //
      bool checkVictory();

      //------------------------------------------------------------------------
      // Parse Map Method
      // Parses a row of the map from a string literal into the map_
      // @param line the string literal to be parsed into the map_
      // @param row the current row that is being parsed
      // @return int 0 on success -1 on illegal layout
      //
      int parseMapRow(string line, int row);

      //------------------------------------------------------------------------
      // The Signum Method
      // Depending on the steps it returns one (right) or minus one (left).
      // @param integer the int to get the sign of
      // @return int -1 for negative, 0 for 0, 1 for positive
      //
      int signum(int integer);

      //------------------------------------------------------------------------
      // Fall Down Method
      // Applies gravity for a worm at a given position
      // @param row the row of the worm
      // @param col the column of the worm
      // @return int row on which the worm stands after applying gravity or -1
      //         if he drowned
      //
      int fallDown(int row, int col);

      //------------------------------------------------------------------------
      // Check Gravity Method
      // Checks whether there is a worm at a given position and applies gravity
      // to him
      // @param row The row of the worm
      // @param col The column of the worm
      //
      void checkGravity(int row, int col);

      //------------------------------------------------------------------------
      // Initialize Map Method
      // Initializes the map with the board_height_ and board_width_, creating
      // all needed objects for it. After this call NO further pointers for the
      // map should be created
      //
      void initMap();

      //------------------------------------------------------------------------
      // Apply Damage Method
      // Applies damage for a worm at a given position
      // @param row The row of the worm
      // @param col The column of the worm
      // @param damage The amount of damage
      // @param damage_name The kind of damage
      // @return Returns FieldType Air if the worm or chest is destroyed, else
      // it returns the previous FieldType at this location.
      //
      Sep::Field::FieldType
      applyDamage(int row, int col, int damage, std::string damage_name);

      //------------------------------------------------------------------------
      // Get Deltas Method
      // Handles the shooting directions (r,l,u,d,ru,lu,rd,ld).
      // @param direction The string direction of the user
      // @param delta_row The change of the row
      // @param delta_col The change of the col
      //
      void getDeltas(std::string direction, int &delta_row, int &delta_col);

      //------------------------------------------------------------------------
      // Out Of Bounds Method
      // Handles out of map behaviour
      // @param row The row
      // @param col The column
      // @return Returns true if it is out of map and false if not
      //
      bool isOutOfBounds(int row, int col);

      //------------------------------------------------------------------------
      // Put In Bounds Method
      // If it is out of map it puts it back.
      // @param row The row
      // @param col The column
      //
      void putInBounds(int &row, int &col);

      //------------------------------------------------------------------------
      // Do Commands Method
      // Sets the right command depending on the input
      // @param command_string Command the user inputed
      // @param acted If the user already did an action
      // @param moved If the user already did move
      // @return Returns a pointer to the command if command is valid, else
      // returns nullpointer
      //
      Command* doCommands(std::string &cmd_string, bool &acted, bool &moved);

      //------------------------------------------------------------------------
      // Change Player Method
      // Manages the player change
      // @param team_a Pointer to the current worm of team a
      // @param team_b Pointer to the current worm of team b
      // @return Returns a pointer to the next worm
      //
      std::shared_ptr<Sep::Worm>  changePlayers(std::shared_ptr<Worm> team_a,
        std::shared_ptr<Worm> team_b);
  };
}

#endif //_GAME_H