// Unit tests for hex game and its data structures using Googletest:
//   http://code.google.com/p/googletest/

#include "hex_game.h"
#include "gtest/gtest.h"

#include <sstream>
#include <string>
#include <unordered_set>

using namespace std;

string position_to_string(const Position& position) {
  stringstream ss;
  ss << position;
  return ss.str();
}

unordered_set<string> get_position_set(const vector<Position>& positions) {
  unordered_set<string> result;
  for(auto& position: positions)
    result.emplace(position_to_string(position));
  return result;
}

void check_position_in_set(const vector<Position>& positions,
			   const unordered_set<string>& neighbor_set) {
  for(auto& position: positions)
    EXPECT_TRUE(neighbor_set.count(position_to_string(position)))
      << position << " should exist in the neighbor set.";
}

TEST(position_test_suite, test_neighbors_corners) {
 
  // North-west corner.
  Position position = Position(0, 0);
  vector<Position> neighbors = { Position(0, 1), Position(1, 0) };
  unordered_set<string> neighbor_set = get_position_set(neighbors);
  Row max_row = MAX_SIZE - 1;
  Column max_column = MAX_SIZE - 1;
  check_position_in_set(position.neighbors(), neighbor_set);

  // North-east corner.
  position = Position(0, max_column);
  neighbors = { Position(0, max_column-1), 
		Position(1, max_column-1),
		Position(1, max_column) };
  neighbor_set = get_position_set(neighbors);
  check_position_in_set(position.neighbors(), neighbor_set);

  // South-west corner.
  position = Position(max_row, 0);
  neighbors = { Position(max_row-1, 0), 
		Position(max_row-1, 1),
		Position(max_row, 1) };
  neighbor_set = get_position_set(neighbors);
  check_position_in_set(position.neighbors(), neighbor_set);

  // South-east corner.
  position = Position(max_row, max_column);
  neighbors = { Position(max_row, max_column-1),
		Position(max_row-1, max_column) };
  neighbor_set = get_position_set(neighbors);
  check_position_in_set(position.neighbors(), neighbor_set);
}

TEST(position_test_suite, test_neighbors_borders) {
 
  // Northern border.
  Position position = Position(0, 1);
  vector<Position> neighbors = { Position(0, 0),
				 Position(0, 2),
				 Position(1, 1),
				 Position(1, 0) };
  unordered_set<string> neighbor_set = get_position_set(neighbors);
  Row max_row = MAX_SIZE - 1;
  Column max_column = MAX_SIZE - 1;
  check_position_in_set(position.neighbors(), neighbor_set);

  // Southern border.
  position = Position(max_row, max_column-1);
  neighbors = { Position(max_row, max_column-2), 
		Position(max_row-1, max_column-1),
		Position(max_row-1, max_column),
		Position(max_row, max_column) };
  neighbor_set = get_position_set(neighbors);
  check_position_in_set(position.neighbors(), neighbor_set);

  // Western border.
  position = Position(2, 0);
  neighbors = { Position(1, 0), 
		Position(1, 1),
		Position(2, 1),
		Position(3, 0) };
  neighbor_set = get_position_set(neighbors);
  check_position_in_set(position.neighbors(), neighbor_set);

  // Eastern border.
  position = Position(3, max_column);
  neighbors = { Position(3, max_column-1), 
		Position(2, max_column),
		Position(4, max_column),
		Position(4, max_column-1) };
  neighbor_set = get_position_set(neighbors);
  check_position_in_set(position.neighbors(), neighbor_set);
}

TEST(position_test_suite, test_neighbors_inner_position) {
  Position position = Position(1, 1);
  vector<Position> neighbors = { Position(1, 0),
				 Position(0, 1),
				 Position(0, 2),
				 Position(1, 2),
				 Position(2, 1),
				 Position(2, 0) };
  unordered_set<string> neighbor_set = get_position_set(neighbors);
  check_position_in_set(position.neighbors(), neighbor_set);
}

TEST(hex_board_test_suite, test_invalid_position) {
  HexBoard hb = HexBoard(7);
  Result expect = { false, INVALID_POSITION };
  string invalid_position_msg = "Hex board should return invalid position.";
  EXPECT_EQ(expect, hb.add_player_position(Player::COMPUTER, 10, 1))
    << invalid_position_msg;
  EXPECT_EQ(expect, hb.add_player_position(Player::CONTESTANT, 0, 7))
    << invalid_position_msg;
}

TEST(hex_board_test_suite, test_add_player_position) {
  HexBoard hb = HexBoard(3);
  string error_msg = "Hex board should return success.";
  EXPECT_EQ(SUCCESS_RESULT, hb.add_player_position(Player::COMPUTER, 0, 0))
    << error_msg;
  EXPECT_EQ(SUCCESS_RESULT, hb.add_player_position(Player::CONTESTANT, 2, 2))
    << error_msg;
}

TEST(hex_board_test_suite, test_occupied_position) {
  HexBoard hb = HexBoard(5);
  Result expect = { false, OCCUPIED_POSITION };
  string error_msg = "Hex board should return occupied position.";
  Row same_row = 1;
  Column same_column = 1;
  hb.add_player_position(Player::COMPUTER, same_row, same_column);
  EXPECT_EQ(expect, hb.add_player_position(Player::CONTESTANT, same_row, same_column))
    << error_msg;
  EXPECT_EQ(expect, hb.add_player_position(Player::COMPUTER, same_row, same_column))
    << error_msg;
}

TEST(hex_board_test_suite, test_game_over_computer_win1) {
  HexBoard hb = HexBoard(4);
  string error_msg = "Hex board should return success.";
  vector<Position> computer_positions = { Position(0, 0),
					  Position(1, 0),
					  Position(2, 0) };
  vector<Position> contestant_positions = { Position(0, 1),
					    Position(1, 1),
					    Position(2, 1) };
  for(auto& position: computer_positions) {
    EXPECT_EQ(SUCCESS_RESULT, hb.add_player_position(Player::COMPUTER,
						     position.row(),
						     position.column()))
      << error_msg;
    EXPECT_EQ(Player::NONE, hb.winner());
  }
  for(auto& position: contestant_positions) {
    EXPECT_EQ(SUCCESS_RESULT, hb.add_player_position(Player::CONTESTANT,
						     position.row(),
						     position.column()))
      << error_msg;
    EXPECT_EQ(Player::NONE, hb.winner());
  }

  // This concludes the game.
  EXPECT_EQ(SUCCESS_RESULT, hb.add_player_position(Player::COMPUTER, 3, 0))
    << error_msg;
  EXPECT_EQ(Player::COMPUTER, hb.winner());
  Result expect = { false, GAME_OVER };
  EXPECT_EQ(expect, hb.add_player_position(Player::CONTESTANT, 0, 2))
      << "Hex board should return game over.";
}

TEST(hex_board_test_suite, test_game_over_contestant_win1) {
  HexBoard hb = HexBoard(3);
  string error_msg = "Hex board should return success.";
  vector<Position> computer_positions = { Position(0, 0),
					  Position(0, 1),
					  Position(0, 2),
					  Position(1, 2),
					  Position(2, 0) };
  vector<Position> contestant_positions = { Position(1, 0),
					    Position(1, 1),
					    Position(2, 1) };
  for(auto& position: computer_positions) {
    EXPECT_EQ(SUCCESS_RESULT, hb.add_player_position(Player::COMPUTER,
						     position.row(),
						     position.column()))
      << error_msg;
    EXPECT_EQ(Player::NONE, hb.winner());
  }
  for(auto& position: contestant_positions) {
    EXPECT_EQ(SUCCESS_RESULT, hb.add_player_position(Player::CONTESTANT,
						     position.row(),
						     position.column()))
      << error_msg;
    EXPECT_EQ(Player::NONE, hb.winner());
  }

  // This concludes the game.
  EXPECT_EQ(SUCCESS_RESULT, hb.add_player_position(Player::CONTESTANT, 2, 2))
    << error_msg;
  EXPECT_EQ(Player::CONTESTANT, hb.winner());
  Result expect = { false, GAME_OVER };
  EXPECT_EQ(expect, hb.add_player_position(Player::COMPUTER, 0, 2))
      << "Hex board should return game over.";
}
