
#include "chessboard-representation.hh"
#include <gtest/gtest.h>

TEST(chessboard_rpr, at)
{
    using namespace board;
    auto rpr = Chessboard_rpr();

    auto piece = rpr.at(Position(File::A, Rank::TWO));
    ASSERT_EQ(piece.value(), std::pair(PieceType::PAWN, Color::WHITE));
}

TEST(chessboard_rpr, at2)
{
    using namespace board;
    auto rpr = Chessboard_rpr();

    auto piece = rpr.at(Position(File::A, Rank::ONE));
    ASSERT_EQ(piece.value(), std::pair(PieceType::ROOK, Color::WHITE));
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


