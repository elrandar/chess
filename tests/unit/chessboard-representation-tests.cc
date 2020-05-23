
#include <gtest/gtest.h>

#include "../../src/chess_engine/board/chessboard-representation.hh"

class chessboard_rpr : public ::testing::Test
{
protected:
    board::Chessboard_rpr rpr;
};

TEST_F(chessboard_rpr, at_correct_position)
{
    using namespace board;

    auto piece = rpr.at(Position(File::A, Rank::TWO));
    ASSERT_EQ(piece.value(), std::pair(PieceType::PAWN, Color::WHITE));
}

TEST_F(chessboard_rpr, at_correct_position_2)
{
    using namespace board;

    auto piece = rpr.at(Position(File::A, Rank::ONE));
    ASSERT_EQ(piece.value(), std::pair(PieceType::ROOK, Color::WHITE));
}

TEST_F(chessboard_rpr, at_blank_position)
{
    auto piece = rpr.at(board::Position(board::File::A, board::Rank::FOUR));
    ASSERT_FALSE(piece.has_value());
}
