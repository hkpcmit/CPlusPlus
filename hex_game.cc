// Homework 3
// Initial implementation of hex game.

#include "hex_game.h"
#include "union_find.h"

#include <assert.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

const int BORDER_LENGTH = 60;  // Border length for hex board display.

// ==========
//  Position
// ==========

// Return all neighboring positions.
vector<Position> Position::neighbors() const {
  vector<PositionDelta> deltas = {{ -1, 0 },
				  { 1, 0 }, 
				  { 0, 1 },
				  { 0, -1 },
				  { -1, 1 },
				  { 1, -1 }};
  vector<Position> result;
  long new_row, new_column;

  for(auto& delta: deltas) {
    new_row = row_ + delta.row;
    new_column = column_ + delta.column;

    // Skip new rows and columns that are out of range. 
    if(new_row < 0 || new_row >= max_size_)
      continue;
    if(new_column < 0 || new_column >= max_size_)
      continue;
    result.push_back(Position(new_row, new_column));
  }
  return result;
}

// ==========
//  HexBoard 
// ==========

// Add and validate a player to a given position on the hext board. 
Result HexBoard::add_player_position(Player player,
				     Row row,
				     Column column) {
  Position position = Position(row, column, size_);

  // Do nothing if game is over.
  if(game_over())
    return { false, GAME_OVER };

  // Validate position.
  Result result = check_add_position(position);
  if(!result.status)
    return result;

  // Add player to the position and check if it wins.
  layout_[position.row()][position.column()] = player;
  positions_occupied_++;
  moves_++;
  update_player_move(player, position);
  check_player_winner(player);
  return SUCCESS_RESULT;
}

// Check if this player is a winner.
bool HexBoard::check_player_winner(Player player) {

  // Check existing winner.
  if(winner_ != Player::NONE)
    return winner_ == player;

  // Check if virtual vertices in union-find share the same root.  If so,
  // this player is a winner.
  switch(player) {
  case Player::COMPUTER:
    if(computer_uf_.find(COMPUTER_NORTH_VERTEX) ==
       computer_uf_.find(COMPUTER_SOUTH_VERTEX)) {
      winner_ = Player::COMPUTER;
      return true;
    }
    return false;
  case Player::CONTESTANT:
    if(contestant_uf_.find(CONTESTANT_EAST_VERTEX) ==
       contestant_uf_.find(CONTESTANT_WEST_VERTEX)) {
      winner_ = Player::CONTESTANT;
      return true;
    }
    return false;
  default:

    // This should not occur.
    assert(false);
  }
}

// Validate position before adding player to a position on the board.
Result HexBoard::check_add_position(Position position) const {

  // Check invalid position.
  if(!position.valid())
    return { false, INVALID_POSITION };

  // Check occupied position.
  if(get_player(position) != Player::NONE)
    return { false, OCCUPIED_POSITION };
  return SUCCESS_RESULT;
}

// Print and draw the hex board.
void HexBoard::draw() const {
  string border = string(BORDER_LENGTH, '-');

  cout << border << endl;
  print_move();
  for(Row row = 0; row < size_; row++) { 
    draw_row(row);
    if(row != size_-1)
      draw_row_separator(row);
  }
  cout << endl << border << endl << endl;
}

// Draw a given row.
void HexBoard::draw_row(Row row) const {
  const string separator = " - ";
  
  cout << setw(2*row) << "";
  for(Column i = 0; i < size_; i++) {
    Position position = Position(row, i);
    cout << get_player_symbol(position);
    if(i == size_-1)
      cout << endl;
    else
      cout << separator;
  }
}

// Draw a row separator.
void HexBoard::draw_row_separator(Row row) const {
  const string separator = " / \\";
  
  cout << setw(2*row+1) << "" << '\\';
  for(int i = 0; i < size_-1; i++)
    cout << separator;
  cout << endl;
}

// Initialize the hex board.
void HexBoard::init_board() {
  for(Row i = 0; i < size_; i++)
    layout_.push_back(vector<Player>(size_, Player::NONE));
}

// Print board move.
void HexBoard::print_move() const {
  cout << "BOARD (";
  if(moves_)
    cout << "after Move " << moves_;
  else 
    cout << "no moves";
  cout << ")" << endl << endl;
}

// Update player union-find based on the player's move along the border.
void HexBoard::update_border_move(Player player, Position& position) {
  Vertex vertex = position_vertex(position);

  switch(player) {
  case Player::COMPUTER:
    if(north_border(position))
      computer_uf_.join(COMPUTER_NORTH_VERTEX, vertex);
    else if(south_border(position))
      computer_uf_.join(COMPUTER_SOUTH_VERTEX, vertex);
    break;
  case Player::CONTESTANT:
    if(east_border(position))
      contestant_uf_.join(CONTESTANT_EAST_VERTEX, vertex);
    else if(west_border(position))
      contestant_uf_.join(CONTESTANT_WEST_VERTEX, vertex);
    break;
  default:

    // This should not occur.
    assert(false);
  }
}

// Update player union-find based on the player's move.
void HexBoard::update_player_move(Player player, Position& position) {
  UnionFind& player_uf = player_union_find(player);
  Vertex this_vertex = position_vertex(position);
  Vertex neighbor_vertex;

  // Iterate over all neighboring positions and add edges connecting the
  // position vertices if needed.
  for(auto& neighbor: position.neighbors()) {
    if(get_player(neighbor) == player) {
      neighbor_vertex = position_vertex(neighbor);
      player_uf.join(neighbor_vertex, this_vertex);
    }
  }

  // Update any border move.
  update_border_move(player, position);
}

// ============
//  PlayerBase
// ============

// Return the player's move inputted by users via cin.  Users
// have up ot MAX_ATTEMPTS to provide a valid position.  O/W,
// this method will return an invalid position, which causes the
// game runner to ignore the move.
Position PlayerBase::move() const {
  string row_string, column_string;
  Row row;
  Column column;

  for(int attempt = 0; attempt < MAX_ATTEMPTS; attempt++) {
    cout << "Enter row number: ";
    getline(cin, row_string);
    if(!is_int(row_string)) {
      cout << "Invalid row number: " << row_string << endl;
      continue;
    }
    cout << "Enter column number: ";
    getline(cin, column_string);
    if(!is_int(column_string)) {
      cout << "Invalid column number: " << column_string << endl;
      continue;
    }
    
    // Return position with valid row and column numbers.
    stringstream(row_string) >> row;
    stringstream(column_string) >> column;
    return Position(row, column);
  }

  // Unable to get valid row and column numbers.  Return invalid
  // position.
  cout << "No valid position from " << name() << " after "
       << MAX_ATTEMPTS << " attempts."  << endl; 
  return Position(MAX_SIZE, MAX_SIZE);
}

// ============
//  GameRunner
// ============

// Attempt to add the player's move to the hex board.
Result GameRunner::add_player_move(PlayerBase& player,
				   Row& row,
				   Column& column) {
  Player pl = player.id();
  Result result;

  for(int attempt = 0; attempt < MAX_ATTEMPTS; attempt++) {
    result = hex_board_.add_player_position(pl, row, column);
    if(result.status)
      return result; 
    cout << "Fail to add move from " << player.name()
	 << " to the hex board due to: "
	 << ErrorCodeString[result.error] << '.' << endl;
    get_player_move(player, row, column);
  }
  return result;
}

// Attempt to get the player's move.
Result GameRunner::get_player_move(PlayerBase& player,
				   Row& row,
				   Column& column) const {
  Position position = player.move(); 

  if(!position.valid()) {
    cout << "Ignore move from " << player.name() << '.' << endl;
    return { false, INVALID_POSITION };
  }
  // Return success result with row and column numbers.
  row = position.row();
  column = position.column();
  return SUCCESS_RESULT;
}

// Run and manage the hex board game.
Player GameRunner::run() {
  vector<PlayerBase> players = { player1_, player2_ };
  Row row;
  Column column;
  Result result;
  Player winner;

  while(!hex_board_.game_over()) {
    for(auto& player: players) {
      hex_board_.draw();
      show_current_player_move(player);
      result = get_player_move(player, row, column);

      // Skip this player if there is no valid move.
      if(!result.status)
	continue;
      result = add_player_move(player, row, column);

      // Skip this player if there is a failure.
      if(!result.status) {
	cout << "Fail to add move from " << player.name()
	     << " after " << MAX_ATTEMPTS << " attempts."  << endl; 
	continue;
      }
      if(hex_board_.game_over())
	break;
      moves_++;
    }
  }
  winner = hex_board_.winner();
  cout << get_player_name(winner) << " wins!" << endl;
  hex_board_.draw();
  return winner;
}

