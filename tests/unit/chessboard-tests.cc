#include "../../src/chess_engine/board/chessboard.hh"
#include <gtest/gtest.h>

TEST(chessboard, king_position_white)
{
    using namespace board;
    auto cb = Chessboard();

    ASSERT_EQ(cb.king_position(), Position(File::E, board::Rank::ONE));
}

TEST(chessboard, king_position_black)
{
    using namespace board;
    auto cb = Chessboard();
    cb.setWhiteTurn(false);

    ASSERT_EQ(cb.king_position(), Position(File::E, board::Rank::EIGHT));
}
