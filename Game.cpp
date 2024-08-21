//------------------------------------------------------------------------------
// Game.cpp

#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <iomanip>
#include "Field.h"
#include "Game.h"
#include "Worm.h"
#include "Random.h"
#include "Chest.h"
#include "Command.h"
#include "MoveCommand.h"
#include "QuitCommand.h"
#include "HelpCommand.h"
#include "StateCommand.h"
#include "ChooseCommand.h"
#include "ActionCommand.h"
#include "MapCommand.h"
#include <map>

using Sep::Game;
using std::string;
using std::make_shared;
using std::dynamic_pointer_cast;
using std::shared_ptr;

//------------------------------------------------------------------------------
// Recurring printed lines
//
const char Game::INVALID_CONFIG[] = "[ERROR] invalid config file\n";
const char Game::INVALID_TARGET[] = "[ERROR] invalid target\n";
const char Game::MOVE_WARNING[] = "[WARNING] can't move further\n";
const char Game::SHOT_MISSED[] = "Shot missed...\n";
const char Game::UNKNOWN_COMMAND[] = "[ERROR] unknown command!\n";
const char Game::WRONG_PARA_COUNT[] = "[ERROR] wrong parameter count!\n";
const char Game::ILLEGAL_COMMAND[] = "[ERROR] command currently not allowed!\n";
const char Game::WRONG_PARAMS[] = "[ERROR] invalid parameter!\n";
const char Game::NO_AMMUNITION[] = "[ERROR] no ammunition\n";
const char Game::TEAM_A_VICTORY[] = "END: Player 1 win!\n";
const char Game::TEAM_B_VICTORY[] = "END: Player 2 win!\n";
const char Game::DRAW[] = "END: Draw\n";
const char Game::SHOT[] = "Shot";
const char Game::TORCH[] = "Torch";
const char Game::ATTACK[] = "Attack";

//------------------------------------------------------------------------------
Game::Game() : board_width_(0), board_height_(0)
{
  map_ = vector < vector < shared_ptr<Field>>>();
}

//------------------------------------------------------------------------------
string Game::trim(string &str)
{
  str.erase(str.find_last_not_of(" \n\r\t") + 1);
  return str;
}

//------------------------------------------------------------------------------
bool Game::isLineEmpty(string line)
{
  const string COMMENT_PREFIX = "#";

  if (line.rfind(COMMENT_PREFIX, 0) == 0)
  {
    return true;
  }

  trim(line);
  return line.empty();
}

//------------------------------------------------------------------------------
int Game::loadConfig(string cfg_file)
{
  std::ifstream file;
  file.open(cfg_file);
  const string MAGIC_VALUE = "#SWORMS";
  const std::regex MAP_REGEX("^MAP: *$");
  const std::regex SIZE_REGEX(R"(^SIZE: *\d\d *\d\d *$)");
  int return_code = SUCCESS_CODE;
  bool magic_value_present = false;
  bool size_present = false;
  bool parse_map = false;
  string line;
  int row = 0;
  while (std::getline(file, line))
  {
    trim(line);
    if (!magic_value_present && line.rfind(MAGIC_VALUE, 0) != 0)
    {
      return_code = INVALID_CONFIG_CODE;
      break;
    }
    else if (!magic_value_present)
    {
      magic_value_present = true;
    }
    else if (isLineEmpty(line))
    {//no need to do anything
    }
    else if (!size_present)
    {
      if (std::regex_match(line, SIZE_REGEX))
      {
        if (parseWidthAndHeight(line) != SUCCESS_CODE)
        {
          return_code = INVALID_CONFIG_CODE;
          break;
        }
        size_present = true;
      }
      else
      {
        return_code = INVALID_CONFIG_CODE;
        break;
      }
    }
    else
    {//by process of elimination, this must be the map.
      if (!parse_map && std::regex_match(line, MAP_REGEX))
      {
        initMap();

        parse_map = true;
      }
      else if (parse_map)
      {
        if (parseMapRow(line, row) != SUCCESS_CODE)
        {
          return_code = INVALID_CONFIG_CODE;
          break;
        }
        row++;
      }
      else
      {
        return_code = INVALID_CONFIG_CODE;
        break;
      }
    }
  }
  if (row != board_height_)
  {
    return_code = INVALID_CONFIG_CODE;
  }
  if (return_code != SUCCESS_CODE)
  {
    std::cout << INVALID_CONFIG;
  }
  return return_code;
}

//------------------------------------------------------------------------------
int Game::parseWidthAndHeight(std::string line)
{
  const std::regex NUMBER_REGEX(R"((\d\d) *(\d\d))");
  std::smatch match;

  if (std::regex_search(line, match, NUMBER_REGEX))
  {
    int temp_width = stoi((string) match[1]);
    int temp_height = stoi((string) match[2]);
    if (temp_width < MIN_SIZE || temp_width > MAX_SIZE || temp_height < MIN_SIZE
        || temp_height > MAX_SIZE)
    {
      return INVALID_CONFIG_CODE;
    }

    board_width_ = temp_width;
    board_height_ = temp_height;
    return SUCCESS_CODE;
  }
  return INVALID_CONFIG_CODE;
}

//------------------------------------------------------------------------------
void Game::initMap()
{
  map_ = vector < vector < shared_ptr<Field> > > (board_height_);

  for (int row = 0; row < board_height_; row++)
  {
    map_[row] = vector<shared_ptr<Field> >
      (static_cast<unsigned long>(board_width_));
    for (int col = 0; col < board_width_; col++)
    {
      map_[row][col] = make_shared<Field>(Field::FieldType::AIR);
    }
  }
}

//------------------------------------------------------------------------------
int Game::parseMapRow(string line, int row)
{
  const char AIR = 'A';
  const char WATER = 'W';
  const char EARTH = 'E';

  trim(line);
  if (line.length() > (unsigned) board_width_)
  {
    return INVALID_CONFIG_CODE;
  }

  for (int index = 0; index < board_width_; index++)
  {
    switch (line[index])
    {
      case AIR:
        if (row == board_height_ - 1)
        {
          return INVALID_CONFIG_CODE;
        }
        //technically not needed as the map is initialized with AIR
        map_[row][index]->setType(Field::FieldType::AIR);
        break;
      case WATER:
        if (row != board_height_ - 1)
        {
          return INVALID_CONFIG_CODE;
        }
        map_[row][index]->setType(Field::FieldType::WATER);
        break;
      case EARTH:
        if (row == 0)
        {
          return INVALID_CONFIG_CODE;
        }
        map_[row][index]->setType(Field::FieldType::EARTH);
        break;
      default:
        return INVALID_CONFIG_CODE;
    }
  }
  return SUCCESS_CODE;
}

//------------------------------------------------------------------------------
void Game::printMap()
{

  std::cout << "Current Map:\n";

  string first_line = "  ";
  for (int count = 0; count < board_width_; count++)
  {
    first_line += std::to_string(count % 10);
  }
  first_line += "\n";

  string line;
  std::ostringstream oss;
  for (int row = 0; row < board_height_; row++)
  {
    oss << std::setw(2) << row;
    for (int column = 0; column < board_width_; column++)
    {
      if (map_[row][column]->getType() == Sep::Field::FieldType::WORM)
      {
        oss << dynamic_pointer_cast<Worm>(map_[row][column])->getTeamChar();
      }
      else
      {
        oss << map_[row][column]->getCharacter();
      }
    }
    oss << std::setw(2) << row << "\n";
  }

  std::cout << first_line << oss.str() << first_line;
}

//------------------------------------------------------------------------------
int Game::spawnWorms(Random &random_object)
{
  int id = 1;
  int col = 0;
  int row = 0;

  while (id < 7)
  {
    std::string name = random_object.getWormName();
    trim(name);
    col = random_object.getRandomInt(0, board_width_ - 1);
    while (map_[row][col]->getType() == Field::FieldType::WORM)
    {
      col = random_object.getRandomInt(0, board_width_ - 1);
    }

    std::cout << "spawning " << name << " (" << id << ") at (" << row << ", "
              << col << ")" << '\n';


    //using shared pointer to easily move the worm in the map
    shared_ptr<Worm> new_worm = make_shared<Worm>(id, name);
    row = fallDown(row, col);
    worms_[id] = {row, col}; //to keep track of the position

    if (row == DROWNED)
    {
      killWorm(new_worm, DROWNING);
    }
    else if (row == FELL_OUT)
    {
      killWorm(new_worm, FALLING);
    }
    else
    {
      map_[row][col] = new_worm;
    }

    id++;
    row = 0;
  }
  return SUCCESS_CODE;
}

//------------------------------------------------------------------------------
void Game::spawn_Chest(Random &random_object)
{

  int col = 0;
  int row = 0;
  shared_ptr<Chest> chest = make_shared<Chest>(random_object, board_width_);
  col = chest->getRow();

  if (map_[0][col]->getType() != Field::FieldType::AIR
      || (row = fallDown(row, col)) < 0)
  {
    return; //unable to spawn chest or is spawned above water
  }

  if (map_[row + 1][col]->getType() == Field::FieldType::WORM)
  {
    //collect if chest spawns above worm, pointer downcast because map
    //contains Field - elements
    dynamic_pointer_cast<Worm>(map_[row + 1][col])
      ->addAmmunition(chest->getLoot());
  }
  else
  {
    map_[row][col] = chest;
  }
}

//------------------------------------------------------------------------------
void Game::killWorm(shared_ptr<Worm> worm, Cause cause)
{
  switch (cause)
  {
    case DROWNING:
      std::cout << worm->getName() << " (" << worm->getID() << ") drowned.\n";
      break;
    case FALL_DAMAGE:
      std::cout << worm->getName() << " (" << worm->getID()
                << ") fell into his death.\n";
      break;
    case FALLING:
      std::cout << worm->getName() << " (" << worm->getID()
                << ") fell out of the map.\n";
      break;
    case WEAPONS:
      std::cout << worm->getName() << " (" << worm->getID() << ") died.\n";
      break;
  }
  worms_.erase(worm->getID()); //when worm dies he gets deleted
}

//------------------------------------------------------------------------------
int Game::move(int id, int steps)
{
  auto x = worms_.find(id);
  if (x == worms_.end()) //if the id is not available
  {
    std::cout << INVALID_TARGET;
    return SUCCESS_CODE;
  }
  int row = x->second[0]; //get the coords for the current id
  int col = x->second[1];

  if (map_[row][col]->getType() != Field::FieldType::WORM)
  {
    std::cout << INVALID_TARGET;
    return SUCCESS_CODE;
  }

  shared_ptr<Worm> move_worm =
    dynamic_pointer_cast<Worm>(map_[row][col]);

  while (steps != 0)
  {
    col += signum(steps);
    map_[row][col - signum(steps)] = make_shared<Field>(Field::FieldType::AIR);
    if (isOutOfBounds(0, col)) //if the worm is out of map, he dies
    {
      killWorm(move_worm, FALLING);
      return MoveCommand::DIED_AFTER_MOVING;
    }

    if (map_[row][col]->getType() == Field::FieldType::AIR
        || map_[row][col]->getType() == Field::FieldType::CHEST)
    {
      if ((row = wormFall(move_worm, row, col)) < 0)
      {
        return MoveCommand::DIED_AFTER_MOVING;
      }

      map_[row][col] = move_worm;

      checkGravity(row, col - signum(steps)); //falling down
      checkGravity(row, col);

      x->second[0] = row;
      x->second[1] = col;

    }
    else if ((map_[row - 1][col]->getType() == Field::FieldType::AIR
              || map_[row - 1][col]->getType() == Field::FieldType::CHEST)
             && map_[row - 1][col - signum(steps)]->getType() !=
                Field::FieldType::EARTH)
    {
      row -= 1;
      if (map_[row][col]->getType() == Field::FieldType::CHEST)
      {
        shared_ptr<Chest> chest = dynamic_pointer_cast<Chest>(map_[row][col]);
        move_worm->addAmmunition(chest->getLoot());
      }
      map_[row][col] = move_worm;
      x->second[0] = row;
      x->second[1] = col;
      checkGravity(row + 1, col - signum(steps));
      checkGravity(row + 1, col);
    }
    else
    {
      map_[row][col - signum(steps)] = move_worm;
      std::cout << MOVE_WARNING;
      return SUCCESS_CODE;
    }
    steps -= signum(steps);
  }
  return SUCCESS_CODE;
}

//------------------------------------------------------------------------------
int Game::signum(int integer)
{
  return (0 < integer) - (integer < 0);
}

//------------------------------------------------------------------------------
int Game::wormFall(shared_ptr<Worm> worm, int row, int col)
{
  if (isOutOfBounds(row, col))
  {
    return row;
  }
  int new_row = row;

  do
  {
    if (map_[new_row][col]->getType() == Field::FieldType::CHEST)
    {
      //collect loot while falling
      worm->addAmmunition(
        dynamic_pointer_cast<Chest>(map_[new_row][col])->getLoot());
    }

    if (new_row + 1 >= board_height_)
    {
      killWorm(worm, FALLING); //worm falls out of map -> dies
      return FELL_OUT;
    }
    else if (map_[new_row + 1][col]->getType() == Field::FieldType::WATER)
    {
      killWorm(worm, DROWNING); //worm drowns
      return DROWNED;
    }


    new_row++;
  } while (map_[new_row][col]->getType() != Field::FieldType::EARTH
           && map_[new_row][col]->getType() != Field::FieldType::WORM);

  //Quick fix
  new_row--;

  if (new_row - row > 1)
  {
    int damage = (new_row - row - 1) * 10;



    if (worm->damage(damage, false))
    {
      killWorm(worm, FALL_DAMAGE); //if falldamage is to high -> worm dies
      return TOO_HIGH;
    }
    else
    {
      std::cout << worm->getName() << " (" << worm->getID() << ") took "
              << damage << "hp fall damage\n";
    }
  }
  return new_row;
}

//------------------------------------------------------------------------------
int Game::fallDown(int row, int col)
{
  //falling down without hp (chest e.g.)
  if (isOutOfBounds(row, col))
  {
    return SUCCESS_CODE;
  }

  while (map_[row + 1][col]->getType() != Field::FieldType::EARTH
    && map_[row + 1][col]->getType() != Field::FieldType::WORM
    && map_[row + 1][col]->getType() != Field::FieldType::CHEST)
  {
    if (map_[row + 1][col]->getType() == Field::FieldType::WATER)
    {
      //drowns
      return DROWNED;
    }
    else if (row >= board_height_)
    {
      //falls out of map
      return FELL_OUT;
    }
    row++;
  }
  return row;
}

//------------------------------------------------------------------------------
void Game::checkGravity(int row, int col)
{
  putInBounds(row, col);
  int new_row;
  while (row > 0)
  {
    if (map_[row][col]->getType() == Field::FieldType::WORM)
    {
      shared_ptr<Worm> falling_worm =
        dynamic_pointer_cast<Worm>(map_[row][col]);
      map_[row][col] = make_shared<Field>(Field::FieldType::AIR);

      if ((new_row = wormFall(falling_worm, row, col)) < 0)
      {
        continue;
      }

      map_[new_row][col] = falling_worm;
      worms_.find(falling_worm->getID())->second[0] = new_row;
    }
    else if (map_[row][col]->getType() == Field::FieldType::CHEST)
    {
      shared_ptr<Chest> falling_chest =
        dynamic_pointer_cast<Chest>(map_[row][col]);
      map_[row][col] = make_shared<Field>(Field::FieldType::AIR);

      if ((new_row = fallDown(row, col)) < 0)
      {
        continue;
      }

      if (map_[new_row + 1][col]->getType() == Field::FieldType::WORM)
      {
        dynamic_pointer_cast<Worm>(map_[new_row + 1][col])
          ->addAmmunition(falling_chest->getLoot());
      }
      else
      {
        map_[new_row][col] = falling_chest;
      }
    }
    row -= 1;
  }
}

//------------------------------------------------------------------------------
Game::~Game()
{
  map_.clear();
}

//------------------------------------------------------------------------
std::shared_ptr<Sep::Worm> Game::changePlayers(std::shared_ptr<Worm> team_a,
  std::shared_ptr<Worm> team_b)
{
  if (current_team_ == Worm::TEAM_A)
  {
    if (team_a == nullptr)
    {
      return team_a = getNextOfTeam(0, Worm::TEAM_A);
    }
    else
    {
      return team_a = getNextOfTeam(team_a->getID(), Worm::TEAM_A);
    }
  }
  else
  {
    if (team_b == nullptr)
    {
      return team_b = getNextOfTeam(3, Worm::TEAM_B);
    }
    else
    {
      return team_b = getNextOfTeam(team_b->getID(), Worm::TEAM_B);
    }
  }
  return nullptr;
}

//------------------------------------------------------------------------------
int Game::mainLoop(Random &random_object)
{
  std::string command_string;
  current_team_ = Worm::TEAM_A;
  shared_ptr<Worm> team_a = nullptr;
  shared_ptr<Worm> team_b = nullptr;
  while (true)
  {
    if (checkVictory())
    {
      return SUCCESS_CODE;
    }
    if(current_team_ == Worm::TEAM_A)
    {
      team_a = changePlayers(team_a, team_b);
    }
    else
    {
      team_b = changePlayers(team_a, team_b);
    }
    current_worm_ = (current_team_ == Worm::TEAM_A) ? team_a : team_b;
    auto coords = worms_.find(current_worm_->getID());
    std::cout << "Player " << ((current_team_ == Worm::TEAM_A) ? 1 : 2)
              << " Worm " << current_worm_->getName() << " ("
              << current_worm_->getID()
              << ") at (" << coords->second[0] << ", " << coords->second[1]
              << ") ready\n";
    current_worm_->equipWeapon(Field::Weapon::GUN);
    Command *command;
    bool moved = false, acted = false;
    while (!acted)
    {
      std::cout << "sep> ";
      std::getline(std::cin, command_string);
      std::transform(command_string.begin(), command_string.end(),
        command_string.begin(), tolower); //case insensitive input
      if (std::cin.bad() || std::cin.eof())
      {
        return SUCCESS_CODE;
      }
      if (trim(command_string).empty())
      {
        continue;
      }
      std::istringstream iss(command_string);
      std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
        std::istream_iterator<std::string>{}};
      if((command = doCommands(tokens[0], acted, moved)) == nullptr)
      {
        continue;
      }
      int ret_value = command->execute(*this, tokens, current_worm_->getID());
      delete (command);
      if (ret_value == Sep::MoveCommand::INVALID_COMMAND)
      {
        moved = false;
      }
      if (moved && (tokens[0] == MoveCommand::MOVE_COMMAND))
      {
        printMap();
      }
      if (ret_value == QuitCommand::QUIT_VALUE)
      {
        return QuitCommand::QUIT_VALUE;
      }
      if (ret_value == ActionCommand::BAD_ACTION)
      {
        acted = false;
      }
      else if (ret_value == MoveCommand::DIED_AFTER_MOVING)
      {
        break;
      }
    }
    spawn_Chest(random_object);
    printMap();
    current_team_ = (current_team_ == Worm::TEAM_A) ? Worm::TEAM_B
      : Worm::TEAM_A;
  }
}

//------------------------------------------------------------------------
Sep::Command* Game::doCommands(std::string &cmd_string, bool &acted,
  bool &moved)
{
  if ((cmd_string == MoveCommand::MOVE_COMMAND))
  {
    if (moved)
    {
      std::cout << ILLEGAL_COMMAND;
      return nullptr;
    }
    moved = true;
    return new MoveCommand();
  }
  else if (cmd_string == QuitCommand::QUIT_COMMAND)
  {
    return new QuitCommand();
  }
  else if (cmd_string == HelpCommand::HELP_COMMAND)
  {
    return new HelpCommand();
  }
  else if (cmd_string == StateCommand::STATE_COMMAND)
  {
    return new StateCommand();
  }
  else if (cmd_string == ChooseCommand::CHOOSE_COMMAND)
  {
    return new ChooseCommand();
  }
  else if (cmd_string == MapCommand::MAP_COMMAND)
  {
    return new MapCommand();
  }
  else if (cmd_string == ActionCommand::ACTION_COMMAND)
  {
    acted = true;
    return new ActionCommand();
  }
  else
  {
    std::cout << UNKNOWN_COMMAND;
    return nullptr;
  }
}

//------------------------------------------------------------------------------
shared_ptr<Sep::Worm> Game::getNextOfTeam(int last_id, char team)
{
  int offset = (team == Worm::TEAM_B) ? 3 : 0;
  int current_id;

  for (int index = 1; index < 4; index++)
  {
    current_id = (index + last_id) % 3;
    if (current_id == 0)
    {
      current_id = 3;
    }
    current_id += offset;

    auto coords = worms_.find(current_id);
    if (coords != worms_.end())
    {
      return dynamic_pointer_cast<Worm>
        (map_[coords->second[0]][coords->second[1]]);
    }
  }
  return nullptr;
}

//------------------------------------------------------------------------------
bool Game::checkVictory()
{
  shared_ptr<Worm> team_a = getNextOfTeam(1, Worm::TEAM_A);
  shared_ptr<Worm> team_b = getNextOfTeam(4, Worm::TEAM_B);
  if (team_a == nullptr && team_b == nullptr)
  {
    std::cout << DRAW;
    return true;
  }
  if (team_a == nullptr)
  {
    std::cout << TEAM_B_VICTORY;
    return true;
  }
  if (team_b == nullptr)
  {
    std::cout << TEAM_A_VICTORY;
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
void Sep::Game::print_state()
{
  std::cout << "current player: " << ((current_team_ == Worm::TEAM_A) ? 1 : 2)
            << "\ncurrent worm: " << current_worm_->getName() << " ("
            << current_worm_->getID() << ") \n\n"
            << current_worm_->getWeaponList()
            << "\nworms of player 1\n";

  bool first_team_b = true;
  for (auto const &element : worms_)
  {
    shared_ptr<Worm> worm = dynamic_pointer_cast<Worm>
      (map_[element.second[0]][element.second[1]]);

    if (first_team_b && worm->getTeamChar() == Worm::TEAM_B)
    {
      std::cout << "worms of player 2\n";
      first_team_b = false;
    }

    std::cout << "  " << worm->getName() << " (" << worm->getID() << ") "
              << worm->getHP() << " HP (" << element.second[0] << ", "
              << element.second[1] << ") " << worm->getTeamChar() << "\n";
  }
}

//------------------------------------------------------------------------------
void Sep::Game::equipWeapon(Sep::Field::Weapon weapon, int worm_id)
{
  auto coords = worms_.find(worm_id);
  shared_ptr<Worm> worm = dynamic_pointer_cast<Worm>
    (map_[coords->second[0]][coords->second[1]]);

  if (weapon != Field::Weapon::GUN && worm->getAmmunition(weapon) < 1)
  {
    std::cout << NO_AMMUNITION;
    return;
  }
  worm->equipWeapon(weapon);
  string ammunition = (worm->getAmmunition(weapon) == std::numeric_limits<int>
  ::max()) ? "inf" : std::to_string(worm->getAmmunition(weapon));

  std::cout << "Chose weapon " << Field::getWeaponName(weapon)
            << " Ammunition: " << ammunition << "\n";
}

//------------------------------------------------------------------------------
int Sep::Game::melee_attack(int worm_id)
{
  auto coords = worms_.find(worm_id);
  int row = coords->second[0];
  int col = coords->second[1];
  shared_ptr<Worm> worm = dynamic_pointer_cast<Worm>(map_[row][col]);

  if (worm->getEquipped() != Field::Weapon::MELEE)
  {
    std::cout << ILLEGAL_COMMAND;
    return ActionCommand::BAD_ACTION;
  }

  worm->useAmmunition(Field::Weapon::MELEE); //ammunition is used

  vector<std::pair<int, int>> coordinates = {{row - 1, col}, {row - 1, col + 1},
    {row, col + 1}, {row + 1, col + 1}, {row + 1, col}, {row + 1, col - 1},
    {row, col - 1}, {row - 1, col - 1}};//8-connected
  int damage = Field::getWeaponDamage(Field::Weapon::MELEE);
  for (auto &coordinate : coordinates)
  {
    if (applyDamage(coordinate.first, coordinate.second, damage, ATTACK)
        != Field::FieldType::AIR)
    {
      checkGravity(coordinate.first, coordinate.second);
    }
  }
  return SUCCESS_CODE;
}

//------------------------------------------------------------------------------
Sep::Field::FieldType Sep::Game::applyDamage(int row, int col, int damage,
  std::string damage_name)
{
  if (isOutOfBounds(row, col))
  {
    return Field::FieldType::AIR;
  }
  Field::FieldType field_type = map_[row][col]->getType();
  if (field_type == Field::FieldType::WORM)
  {
    shared_ptr<Worm> worm = dynamic_pointer_cast<Worm>(map_[row][col]);

    std::cout << damage_name << " hit " << Field::getFieldName(field_type)
              << " at position (" << row << ", " << col << ")\n";

    if (worm->damage(damage, true))
    {
      map_[row][col] = make_shared<Field>(Field::FieldType::AIR);
      killWorm(worm, Cause::WEAPONS);
    }
    return field_type;
  }

  if (damage_name != ATTACK && (field_type == Field::FieldType::EARTH
                                || field_type == Field::FieldType::CHEST))
  {
    std::cout << damage_name << " hit " << Field::getFieldName(field_type)
              << " at position (" << row << ", " << col << ")\n";
    map_[row][col] = std::make_shared<Field>(Field::FieldType::AIR);
    return field_type;
  }
  return Field::FieldType::AIR;
}

//------------------------------------------------------------------------------
int Sep::Game::teleport(int row, int col, int worm_id)
{
  auto coords = worms_.find(worm_id);
  int old_row = coords->second[0];
  int old_col = coords->second[1];
  shared_ptr<Worm> worm = dynamic_pointer_cast<Worm>(map_[old_row][old_col]);

  if (worm->getEquipped() != Field::Weapon::TELEPORT)
  {
    std::cout << ILLEGAL_COMMAND;
    return ActionCommand::BAD_ACTION;
  }
  if (isOutOfBounds(row, col))
  {
    std::cout << INVALID_TARGET;
    return ActionCommand::BAD_ACTION;
  }
  Field::FieldType field_type = map_[row][col]->getType();

  if (field_type != Field::FieldType::AIR && field_type
                                             != Field::FieldType::CHEST)
  {
    std::cout << INVALID_TARGET;
    return ActionCommand::BAD_ACTION;
  }

  worm->useAmmunition(Field::Weapon::TELEPORT);
  map_[old_row][old_col] = make_shared<Field>(Field::FieldType::AIR);
  checkGravity(old_row, old_col);

  if ((row = wormFall(worm, row, col)) >= 0)
  {
    worms_.find(worm_id)->second[0] = row;
    worms_.find(worm_id)->second[1] = col;
    map_[row][col] = worm;
    checkGravity(row, col);
  }
  return SUCCESS_CODE;
}

//------------------------------------------------------------------------------
int Sep::Game::airstrike(int col, int worm_id)
{
  auto coords = worms_.find(worm_id);
  shared_ptr<Worm> worm = dynamic_pointer_cast<Worm>(map_[coords->second[0]]
  [coords->second[1]]);

  if (worm->getEquipped() != Field::Weapon::AIRSTRIKE)
  {
    std::cout << ILLEGAL_COMMAND;
    return ActionCommand::BAD_ACTION;
  }

  if (isOutOfBounds(0, col))
  {
    std::cout << INVALID_TARGET;
    return ActionCommand::BAD_ACTION;
  }

  worm->useAmmunition(Field::Weapon::AIRSTRIKE);
  int row = 0;
  int damage = Field::getWeaponDamage(Field::Weapon::AIRSTRIKE);
  while (applyDamage(row, col, damage, SHOT) == Field::FieldType::AIR)
  {
    row++;
    if (isOutOfBounds(row, 0))
    {
      std::cout << SHOT_MISSED;
      return SUCCESS_CODE;
    }
  }
  checkGravity(row, col);
  return SUCCESS_CODE;
}

//------------------------------------------------------------------------------
int Sep::Game::shoot_weapon(std::string direction, int worm_id)
{
  auto coords = worms_.find(worm_id);
  int row = coords->second[0];
  int col = coords->second[1];
  shared_ptr<Worm> worm = dynamic_pointer_cast<Worm>(map_[row][col]);
  Field::Weapon weapon = worm->getEquipped();
  int damage = Field::getWeaponDamage(weapon);
  int delta_row, delta_col;

  getDeltas(std::move(direction), delta_row, delta_col); //direction of shoot
  row += delta_row;
  col += delta_col;

  if (weapon == Field::Weapon::GUN || weapon == Field::Weapon::BAZOOKA)
  {
    worm->useAmmunition(weapon);
    while (applyDamage(row, col, damage, SHOT) == Field::FieldType::AIR)
    {
      row += delta_row;
      col += delta_col;
      if (isOutOfBounds(row, col))
      {
        std::cout << SHOT_MISSED;
        return SUCCESS_CODE;
      }
    }

    if (weapon == Field::Weapon::BAZOOKA)
    {
      applyDamage(row - 1, col, damage, SHOT); //4-connected
      applyDamage(row, col + 1, damage, SHOT);
      applyDamage(row + 1, col, damage, SHOT);
      applyDamage(row, col - 1, damage, SHOT);
      checkGravity(row + 1, col - 1);
      checkGravity(row + 1, col + 1);
    }

    checkGravity(row + 1, col); //checks if something has to fall down
  }
  else if (weapon == Field::Weapon::BLOWTORCH)
  {
    worm->useAmmunition(weapon);
    int counter;
    for (counter = 0; counter < 5; counter++)
    {
      if (isOutOfBounds(row, col))
      {
        break;
      }
      applyDamage(row, col, damage, TORCH);
      row += delta_row;
      col += delta_col;
    }
    //reverse the prior loop for gravity
    //could be integrated into prior loop, but would clash with specification
    for (; counter > 0; counter--)
    {
      row -= delta_row;
      col -= delta_col;
      checkGravity(row, col);
    }
  }
  else
  {
    std::cout << ILLEGAL_COMMAND;
    return ActionCommand::BAD_ACTION;
  }
  return SUCCESS_CODE;
}

//------------------------------------------------------------------------------
void Sep::Game::getDeltas(std::string direction, int &delta_row, int &delta_col)
{
  const string LEFT = "l";
  const string RIGHT = "r";
  const string DOWN = "d";
  const string UP = "u";
  const string LEFT_DOWN = "ld";
  const string RIGHT_DOWN = "rd";
  const string LEFT_UP = "lu";
  const string RIGHT_UP = "ru";

  if (direction == LEFT)
  {
    delta_row = 0;
    delta_col = -1;
  }
  else if (direction == RIGHT)
  {
    delta_row = 0;
    delta_col = 1;
  }
  else if (direction == DOWN)
  {
    delta_row = 1;
    delta_col = 0;
  }
  else if (direction == UP)
  {
    delta_row = -1;
    delta_col = 0;
  }
  else if (direction == LEFT_DOWN)
  {
    delta_row = 1;
    delta_col = -1;
  }
  else if (direction == RIGHT_DOWN)
  {
    delta_row = 1;
    delta_col = 1;
  }
  else if (direction == LEFT_UP)
  {
    delta_row = -1;
    delta_col = -1;
  }
  else if (direction == RIGHT_UP)
  {
    delta_row = -1;
    delta_col = 1;
  }
}

//------------------------------------------------------------------------------
bool Sep::Game::isOutOfBounds(int row, int col)
{
  return (row >= board_height_ || row < 0 || col >= board_width_ || col < 0);
}

//------------------------------------------------------------------------------
void Sep::Game::putInBounds(int &row, int &col)
{
  if (row >= board_height_)
  {
    row = board_height_ - 1;
  }
  else if (row < 0)
  {
    row = 0;
  }

  if (col >= board_width_)
  {
    col = board_width_ - 1;
  }
  else if (col < 0)
  {
    col = 0;
  }
}
