// Mock tests for hex board game using Google Mock:
//   http://code.google.com/p/googlemock/

#include "mock_hex_game.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace testing;

TEST(game_runner_test_suite, test_computer_win1) {
  HexBoard hb = HexBoard(2);
  MockComputer computer;
  EXPECT_CALL(computer, move())
    .Times(2)
    .WillOnce(Return(Position(0, 0)))
    .WillOnce(Return(Position(1, 0)));
  MockContestant contestant;
  EXPECT_CALL(contestant, move())
    .Times(2)
    .WillOnce(Return(Position(0, 1)))
    .WillOnce(Return(Position(1, 1)));
  GameRunner runner = GameRunner(hb, computer, contestant);
  EXPECT_EQ(COMPUTER, runner.run());
}

int main(int argc, char** argv) {
  InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
