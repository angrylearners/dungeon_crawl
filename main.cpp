//
// Created by 31838 on 8/8/2019.
//

#include <iostream>
#include <array>
#include <vector>
#include <random>
#include <chrono>

using std::cout, std::endl, std::cin, std::array, std::vector, std::string;

auto panic(const string &msg) -> void {
  cout << msg << endl;
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

enum class Flag {
  kSuccess,
  kFail,
  kNotOver
};

template<size_t row_num = 10, size_t col_num = 10, size_t trap_num = 10>
class Game {
private:
  Player<row_num, col_num> player_{};
  vector<Position> traps_{};
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

public:
  Game() {
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
      traps_.push_back(trap);
    }
    
    gen_random_trap:
    auto trap = RandomPosition();
    if (!ValidateTrapPosition(trap))
      goto gen_random_trap;
    randomTrap_ = trap;
  }
  
  auto ShowBoard() -> void {
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
  
  auto GetPlayer() {
    return &player_;
  }
  
  auto IsGameOver() -> Flag {
    if (player_ == treasure_)
      return Flag::kSuccess;
    if (player_ == randomTrap_)
      return Flag::kFail;
    for (auto trap:traps_) {
      if (trap == player_)
        return Flag::kFail;
    }
    
    return Flag::kNotOver;
  }
  
  auto GenerateRandomTrap() -> void {
    gen_random_trap:
    auto trap = RandomPosition();
    if (!ValidateTrapPosition(trap))
      goto gen_random_trap;
    randomTrap_ = trap;
  }
};

template<size_t row_num = 10, size_t col_num = 10, size_t trap_num = 10>
auto PlayGame() -> void {
  Game<row_num, col_num, trap_num> g;
  auto player = g.GetPlayer();
  g.ShowBoard();
  play_game:
  
  char command;
  cin >> command;
  switch (command) {
    case 'w':
      player->GoUp();
      break;
    case 's':
      player->GoDown();
      break;
    case 'a':
      player->GoLeft();
      break;
    case 'd':
      player->GoRight();
      break;
    default:
      panic(&"unknown command "[command]);
  }
  g.ShowBoard();
  g.GenerateRandomTrap();
  switch (g.IsGameOver()) {
    case Flag::kSuccess:
      cout << "Wow! You win!" << endl;
      return;
    case Flag::kFail:
      cout << "Sorry. You failed." << endl;
      return;
    case Flag::kNotOver:
      goto play_game;
  }
  
}

auto main() -> int {
  cout << "Welcome to Dungeon Crawl Game!" << endl;
  
  play_game:
  PlayGame();
  
  ask:
  {
    cout << "Would you like to play again? [y/n] ";
    std::string str;
    cin >> str;
    if (str == "y")
      goto play_game;
    else if (str == "n") {
      cout << "Bye Bye." << endl;
      return EXIT_SUCCESS;
    } else {
      cout << "Please enter y or n." << endl;
      goto ask;
    }
  }
}
