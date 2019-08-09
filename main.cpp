//
// Created by 31838 on 8/8/2019.
//

#include <iostream>
#include <array>
#include <random>
#include <chrono>

using std::cout, std::endl, std::cin, std::getchar;
using std::array, std::string;

const size_t row_num = 10, col_num = 10, trap_num = 10;

auto panic(const string &msg) -> void {
  cout << "\033[2J\033[1;1H";
  cout << "[PANIC] " + msg << endl;
  exit(EXIT_FAILURE);
}

class Position {
protected:
  uint row;
  uint col;
public:
  auto GetRow() -> uint {
    return row;
  }
  
  auto GetCol() -> uint {
    return col;
  }
  
  auto SetRow(uint r) -> void {
    row = r;
  }
  
  auto SetCol(uint c) -> void {
    col = c;
  }
  
  auto operator==(const Position &pos) -> bool {
    return pos.row == row && pos.col == col;
  }
  
  auto operator!=(const Position &pos) -> bool {
    return !operator==(pos);
  }
};

template<size_t row_num, size_t col_num>
class Player : public Position {
  auto MoveOutOfBoard() {
    panic("You are moving out of board!");
  }

public:
  auto GoUp() -> void {
    if (!row)
      MoveOutOfBoard();
    row--;
  }
  
  auto GoDown() -> void {
    row++;
    if (row >= row_num)
      MoveOutOfBoard();
  }
  
  auto GoLeft() -> void {
    if (!col)
      MoveOutOfBoard();
    col--;
  }
  
  auto GoRight() -> void {
    col++;
    if (col >= col_num)
      MoveOutOfBoard();
  }
};

enum class GameOverFlag {
  kSuccess,
  kFail,
  kNotOver
};

enum class KeyboardCommand {
  kUp,
  kDown,
  kLeft,
  kRight
};

template<size_t row_num = 10, size_t col_num = 10, size_t trap_num = 10>
class Game {
private:
  Player<row_num, col_num> player_{};
  array<Position, trap_num> traps_{};
  Position randomTrap_{};
  Position treasure_{};
  
  auto RandomPosition() -> Position {
    using std::chrono::system_clock, std::uniform_int_distribution, std::default_random_engine;
    
    auto now = system_clock::now();
    auto dist = uniform_int_distribution<size_t>();
    auto engine = default_random_engine(now.time_since_epoch().count());
    
    Position pos = {};
    pos.SetRow(dist(engine) % row_num);
    pos.SetCol(dist(engine) % col_num);
    return pos;
  }
  
  auto ValidateTrapPosition(Position pos) -> bool {
    return pos != player_ && pos != treasure_;
  }
  
  auto ValidateTreasurePosition(Position pos) -> bool {
    return pos != player_;
  }
  
  auto Init() -> void {
    ClearScreen();
    player_.SetCol(0);
    player_.SetRow(0);
    
    gen_treasure:
    treasure_ = RandomPosition();
    if (!ValidateTreasurePosition(treasure_))
      goto gen_treasure;
    
    for (size_t i = 0; i < trap_num; i++) {
      gen_trap:
      auto trap = RandomPosition();
      if (!ValidateTrapPosition(trap))
        goto gen_trap;
      traps_[i] = trap;
    }
    
    gen_random_trap:
    auto trap = RandomPosition();
    if (!ValidateTrapPosition(trap))
      goto gen_random_trap;
    randomTrap_ = trap;
  }

public:
  Game() {
    Init();
  }
  
  ~Game() {
    ClearScreen();
  }

private:
  auto ClearScreen() -> void {
    cout << "\033[2J\033[1;1H";
  }
  
  auto ShowBoard() -> void {
    ClearScreen();
    array<array<char, col_num>, row_num> board = {};
    
    for (array<char, col_num> &row:board) {
      for (char &col:row)
        col = '.';
    }
    
    for (auto trap:traps_)
      board[trap.GetRow()][trap.GetCol()] = 'T';
    board[randomTrap_.GetRow()][randomTrap_.GetCol()] = 'T';
    board[treasure_.GetRow()][treasure_.GetCol()] = 'X';
    board[player_.GetRow()][player_.GetCol()] = 'G';
    
    for (array<char, col_num> row:board) {
      for (char col:row)
        cout << col;
      cout << endl;
    }
  }
  
  auto IsGameOver() -> GameOverFlag {
    if (player_ == treasure_)
      return GameOverFlag::kSuccess;
    if (player_ == randomTrap_)
      return GameOverFlag::kFail;
    for (auto trap:traps_) {
      if (trap == player_)
        return GameOverFlag::kFail;
    }
    
    return GameOverFlag::kNotOver;
  }
  
  auto GenerateRandomTrap() -> void {
    gen_random_trap:
    auto trap = RandomPosition();
    if (!ValidateTrapPosition(trap))
      goto gen_random_trap;
    randomTrap_ = trap;
  }
  
  auto DoCommand(KeyboardCommand cmd) {
    switch (cmd) {
      case KeyboardCommand::kUp:
        player_.GoUp();
        break;
      case KeyboardCommand::kDown:
        player_.GoDown();
        break;
      case KeyboardCommand::kLeft:
        player_.GoLeft();
        break;
      case KeyboardCommand::kRight:
        player_.GoRight();
        break;
    }
  }
  
  auto Reset() -> void {
    Init();
  }

public:
  auto Play() -> void {
    GameOverFlag flag;
    do {
      GenerateRandomTrap();
      ShowBoard();
      char ch;
      cin >> ch;
      switch (char(ch)) {
        case 'w':
          DoCommand(KeyboardCommand::kUp);
          break;
        case 's':
          DoCommand(KeyboardCommand::kDown);
          break;
        case 'a':
          DoCommand(KeyboardCommand::kLeft);
          break;
        case 'd':
          DoCommand(KeyboardCommand::kRight);
          break;
        case 'r':
          Reset();
          break;
        default:
          panic("Unknown key from Keyboard");
      }
      ShowBoard();
      flag = IsGameOver();
    } while (flag == GameOverFlag::kNotOver);
    
    if (flag == GameOverFlag::kFail)
      cout << "Sorry, you failed" << endl;
    else
      cout << "Wow! You win!" << endl;
  }
};

auto main() -> int {
  cout << "\033[2J\033[1;1H";
  cout << R"(
Welcome to dungeon crawl game.
You can use w,s,a,d to control player and r to reset the game.
Ready to go? Press ENTER to move on!
)";
  getchar();
  
  play:
  Game g = Game<row_num, col_num, trap_num>();
  g.Play();
  ask:
  {
    cout << "Would you like to play again? [y/n] ";
    std::string str;
    cin >> str;
    if (str == "y")
      goto play;
    else if (str == "n") {
      cout << "Bye Bye." << endl;
      return EXIT_SUCCESS;
    } else {
      cout << "Please enter y or n." << endl;
      goto ask;
    }
  }
}
