// Mock classes for hex board game using Google Mock:
//   http://code.google.com/p/googlemock/

#ifndef MOCK_HEX_GAME_H_
#define MOCK_HEX_GAME_H_

#include "hex_game.h"
#include "gmock/gmock.h"

class MockComputer : public PlayerBase {
 public:
  MockComputer() : PlayerBase(COMPUTER) {}
  MOCK_METHOD0(move, Position());
};

class MockContestant : public PlayerBase {
 public:
  MockContestant() : PlayerBase(CONTESTANT) {}
  MOCK_METHOD0(move, Position());
};

#endif // MOCK_HEX_GAME_H_
