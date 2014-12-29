// Homework 4
// Implement hex game.

#include "hex_game.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

const int BOARD_SIZE = 11;       // Hex board size.
const int HDR_WIDTH = 55;        // Header width.
const int ITEM_WIDTH = 17;       // Field width for key item.
const int NOTES_WIDTH = 21;      // Field width for notes.
const int STAR_WIDTH = 3;        // Field width for star.
const int SYMBOL_WIDTH = 7;      // Field width for symbol.

// Get player symbol string for header display.
inline string player_symbol_string(Player p) {
  ostringstream ss;
  ss << player_symbol[static_cast<int>(p)] << "  ";
  return ss.str();
}

// Print game header info.
void print_header_info(string item, string symbol, string notes) {
  char star = '*';
  char separator = ' ';
  
  cout << star << setw(ITEM_WIDTH) << setfill(separator) << item
       << setw(STAR_WIDTH) << setfill(separator) << star
       << setw(SYMBOL_WIDTH) << setfill(separator) << symbol
       << setw(STAR_WIDTH) << setfill(separator) << star
       << setw(NOTES_WIDTH) << setfill(separator) << notes
       << setw(STAR_WIDTH) << setfill(separator) << star << endl;
}

// Print game header.
void print_header() {
  char star = '*';
  char separator = ' ';
  string stars = string(HDR_WIDTH, star);

  cout << stars << endl;
  print_header_info("KEY ITEM   ", "SYMBOL", "NOTES       ");
  cout << stars << endl;
  print_header_info("Empty Location", player_symbol_string(Player::NONE), "");
  print_header_info("Computer Piece", player_symbol_string(Player::COMPUTER),
		    "Connects North-South");
  print_header_info("Contestant Piece", player_symbol_string(Player::CONTESTANT),
		    "Connects East-West ");
  cout << stars << endl;
  cout << endl;
}

// Main routine.
int main() {
  HexBoard hb = HexBoard(BOARD_SIZE);
  PlayerBase computer = PlayerBase(Player::COMPUTER);
  PlayerBase contestant = PlayerBase(Player::CONTESTANT);
  GameRunner runner = GameRunner(hb, computer, contestant);

  print_header();
  runner.run();

  return 0;
}

