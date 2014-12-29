// Header file for Hex game and its data structures.

#ifndef HEX_GAME_H_
#define HEX_GAME_H_

#include "union_find.h"

#include <assert.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const uint32_t MAX_SIZE = 100;  // Maximum hex board size.

typedef long Vertex;      // Player vertex type.
typedef uint32_t Row;     // Hex board row.
typedef uint32_t Column;  // Hex board column.

// Player types, names and symbols.
enum class Player { COMPUTER, CONTESTANT, NONE };
const string player_name[] = { "Computer",
			       "Contestant",
			       "None" };
const char player_symbol[] = { '0', 'X', '.' };

// Virtual vertices.
// Terminating vertex for computer at the northern border.
const Vertex COMPUTER_NORTH_VERTEX = -1;
// Terminating vertex for computer at the southern border.
const Vertex COMPUTER_SOUTH_VERTEX = -2;
// Terminating vertex for contestant at the western border.
const Vertex CONTESTANT_WEST_VERTEX = -3;
// Terminating vertex for contestant at the eastern border.
const Vertex CONTESTANT_EAST_VERTEX = -4;
// Unit cost for edges connecting position vertices. 
const int UNIT_COST = 1;
// Maximum attempts.
const int MAX_ATTEMPTS = 3;

// Failure codes.
enum ErrorCode {
  SUCCESS,              // No failure.
  GAME_OVER,            // Game over.
  INVALID_PLAYER,       // Invalid player.
  INVALID_POSITION,     // Invalid position for player on the hex board. 
  OCCUPIED_POSITION,    // Position on the hex board is already occupied.
};
const string ErrorCodeString[] = { "Success",
				   "Game Over",
				   "Invalid player",
				   "Invalid position",
				   "Position already occupied" };

// Position delta encapsulates the deltas in row and column of hex board
// position.
struct PositionDelta {
  int row, column;
};

// Result of an action on the hex board and by the game runner.  Possible
// actions include:
// - Add player move to the hex board.
// - Check position before adding player to a given position.  
struct Result {
  // True for pass; False for fail.
  bool status;
  // Error code for failure.
  ErrorCode error;
  // For equality check.
  bool operator==(const Result& other) const {
    return this->status == other.status && this->error == other.error; 
  }
};

const Result SUCCESS_RESULT = { true, SUCCESS };  // Success result.

// Return player's name.
inline string get_player_name(Player player) {
  return player_name[static_cast<int>(player)];
}

// Determine if the given string is an integer.
inline bool is_int(string& st) {
  if(st.empty())
    return false;
  for(auto& ch: st)
    if(!isdigit(ch))
      return false;
  return true;
}

// A class to encapsulate the row & column of the hex board position.
class Position {
 public:
  // Construct a board position.
  Position(Row row, Column column, uint32_t max_size=MAX_SIZE)
    : max_size_(max_size),
      row_(row),
      column_(column) {}
  // Get column.
  Column column() const { return column_; }
  // Get row.
  Row row() const { return row_; }
  // Return all neighboring positions.
  vector<Position> neighbors() const;
  // Check if this is a valid position.
  bool valid() const {
    return row_ < max_size_ && column_ < max_size_;
  }
  // Convert position to string.
  friend ostream& operator<<(ostream& out, const Position& position) {
    return out << '(' << position.row() << ", " << position.column() << ')'; 
  }
  // For equality check.
  bool operator==(const Position& other) const {
    return this->row_ == other.row() && this->column_ == other.column();
  }

 private:
  uint32_t max_size_;
  Column column_;
  Row row_;
};

// A class to represent a hex board.  It performs the following actions:
// - Add and validate a player to a given position on the board.
// - Determine winner.
// - Print and draw the board.
class HexBoard {
 public:
  // Construct a hex board.
  HexBoard(uint32_t size)
    : computer_uf_(UnionFind()),
      contestant_uf_(UnionFind()),
      moves_(0),
      positions_occupied_(0),
      size_(size),
      max_column_(size-1),
      max_row_(size-1),
      total_positions_(size*size),
      winner_(Player::NONE) { init_board(); }
  // Add and validate a player to a given position on the board.
  Result add_player_position(Player player, Row row, Column column);
  // Print and draw the hex board.
  void draw() const;
  // Indicate if the game is over.
  bool game_over() const {
    return winner() != Player::NONE || positions_occupied_ == total_positions_; 
  }
  // Get player who occupies a given position.
  Player get_player(Position& position) const {
    return layout_[position.row()][position.column()];
  }
  // Get winner.
  Player winner() const { return winner_; }

 private:
  // Validate position before adding player to a position on the board.
  Result check_add_position(Position position) const;
  // Check if this player is a winner.  This method uses union-find for the
  // player and check if there is a path between the virtual vertices.
  bool check_player_winner(Player player);
  // Draw a given row.
  void draw_row(Row row) const;
  // Draw a row separator following a given row.
  void draw_row_separator(Row row) const;
  // Get player symbol on the given board position.
  char get_player_symbol(Position& position) const {
    int pl = static_cast<int>(get_player(position));
    return player_symbol[pl];
  }
  // Initialize the hex board.
  void init_board();
  // Check if the position is on the easthern border of the board.
  bool east_border(Position& position) const {
    return position.column() == max_column_;
  }
  // Check if the position is on the northern border of the board.
  bool north_border(Position& position) const {
    return position.row() == 0;
  }
  // Check if the position is on the southern border of the board.
  bool south_border(Position& position) const {
    return position.row() == max_row_;
  }
  // Check if the position is on the westhern border of the board.
  bool west_border(Position& position) const {
    return position.column() == 0;
  }
  // Return player's union-find.
  UnionFind& player_union_find(Player player) {
    switch(player) {
    case Player::COMPUTER: return computer_uf_;
    case Player::CONTESTANT: return contestant_uf_;
    default: assert(false);
    }
  }
  // Vertex mapping for a given board position.
  Vertex position_vertex(Position& position) {
    return position.row() * size_ + position.column();
  }
  // Print board move.
  void print_move() const;
  // Update player union-find based on the player's move along the border.
  void update_border_move(Player player, Position& position);
  // Update player union-find based on the player's move.
  void update_player_move(Player player, Position& position);
  // Union-find's for computer and contestant.
  UnionFind computer_uf_, contestant_uf_;
  // Board layout.
  vector<vector<Player> > layout_;
  // Max column number.
  Column max_column_;
  // Max row number.
  Row max_row_;
  // Number of moves.
  uint32_t moves_;
  // Number of positions occupied.
  uint32_t positions_occupied_;
  // Total number of positions on the board.
  uint32_t total_positions_;
  // Board size.
  uint32_t size_;
  // Winner identifier.
  Player winner_;
};

// Base class for the players of hex board game.  In this assignment,
// it will instantiate both computer & contestant and provide the
// player's moves inputted by users via cin.  In the next homework,
// this class will be refactored and derived classes will be developed
// for both computer and contestant.
class PlayerBase {
 public:
  // Construct a player.
  PlayerBase(Player player) : player_(player) {}
  // Destructor of PlayerBase.
  virtual ~PlayerBase() {}
  // Return player identifier.
  Player id() const { return player_; }
  // Return the player's move.  In this assignment, the row and column
  // number will be provided by users via cin.  Users will have up to
  // MAX_ATTEMPTS to provide valid row and column numbers.  If no valid
  // row or column number is provided, the method will return an invalid
  // position, which causes the game runner to ignore this player's move.
  Position move() const;
  // Return player name.
  string name() const { return get_player_name(player_); }

 private:
  Player player_;
};

// A class to run and manage the hex board game.  It performs the following
// actions:
// - Take turn getting the players' moves and add them to the hex board.
// - If any player's move cannot be added to the hex board, try to get
//   another move from the same player up to MAX_ATTEMPTS.
// - Announce winner and stop the game. 
class GameRunner {
 public:
  // Construct a game runner.
  GameRunner(HexBoard& hb, PlayerBase& player1, PlayerBase& player2)
    : hex_board_(hb),
      player1_(player1),
      player2_(player2),
      moves_(1) {}
  // Run and manage the hex board game.  Return the winner after game over.
  Player run();
 private:
  // Attempt to add player's move to the hex board.  If the hex board returns
  // failure, this method tries to get another move from the same player up
  // to MAX_ATTEMPTS and report the final result.
  Result add_player_move(PlayerBase& player, Row& row, Column& column);
  // Attempt to get the player's move.  If a move provided is invalid, this
  // method tries to get another move from the same player up to MAX_ATTEMPTS
  // and report the final result.  Caller should check if the result is
  // successful.  If so, the row and column numbers are valid.  O/W, the
  // numbers should be ignored.
  Result get_player_move(PlayerBase& player, Row& row, Column& column) const;
  // Display move number and current player.
  void show_current_player_move(PlayerBase& player) const {
    int pl = static_cast<int>(player.id());
    cout << "Move " << moves_ << ": " << player.name()
	 << '(' << player_symbol[pl] << ')' << endl;
  }
  HexBoard hex_board_;
  PlayerBase player1_, player2_;
  int moves_;
};

#endif // HEX_GAME_H_
