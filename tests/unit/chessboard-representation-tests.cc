
#include "../../src/chess_engine/board/chessboard-representation.hh"
#include <gtest/gtest.h>


class chessboard_rpr : public ::testing::Test {
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

TEST_F(chessboard_rpr, execute_move)
{
    using namespace board;
    auto pos1 = Position(File::A, Rank::TWO);
    auto pos2 = Position(File::A, Rank::FOUR);
    rpr.execute_move(Move(pos1, pos2, PieceType::PAWN));

    EXPECT_FALSE(rpr.at(pos1).has_value());
    EXPECT_EQ(rpr.at(pos2).value(), std::pair(PieceType::PAWN, Color::WHITE));
}

TEST_F(chessboard_rpr, execute_move_capture)
{
    using namespace board;
    auto pos1 = Position(File::A, Rank::TWO);
    auto pos2 = Position(File::A, Rank::SEVEN);
    rpr.execute_move(Move(pos1, pos2, PieceType::PAWN));

    EXPECT_FALSE(rpr.at(pos1).has_value());
    EXPECT_EQ(rpr.at(pos2).value(), std::pair(PieceType::PAWN, Color::WHITE));
}
