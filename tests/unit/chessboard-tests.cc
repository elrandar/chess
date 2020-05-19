#include "../../src/chess_engine/board/chessboard.hh"
#include "../../src/chess_engine/board/magic.hh"
#include "../../src/chess_engine/board/masks.hh"
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
TEST(chessboard, do_execute_move)
{
    using namespace board;
    auto chessboard = Chessboard();
    auto pos1 = Position(File::A, Rank::TWO);
    auto pos2 = Position(File::A, Rank::FOUR);
    auto move = Move(pos1, pos2, board::PieceType::PAWN);
    chessboard.do_move(move);

    EXPECT_FALSE(chessboard.getBoardRpr().at(pos1).has_value());
    EXPECT_EQ(chessboard.getBoardRpr().at(pos2).value(), std::pair(PieceType::PAWN, Color::WHITE));
}

TEST(chessboard, do_move_capture)
{
    using namespace board;
    auto chessboard = Chessboard();
    auto &rpr = chessboard.getBoardRpr();
    auto pos1 = Position(File::A, Rank::TWO);
    auto pos2 = Position(File::A, Rank::SEVEN);
    auto move = Move(pos1, pos2, board::PieceType::PAWN);
    move.setCapture(PieceType::PAWN);
    chessboard.do_move(move);

    EXPECT_FALSE(rpr.at(pos1).has_value());
    EXPECT_EQ(rpr.at(pos2).value(), std::pair(PieceType::PAWN, Color::WHITE));
}

TEST(chessboard, do_move_undo)
{
    using namespace board;
    auto chessboard = Chessboard();
    auto pos1 = Position(File::A, Rank::TWO);
    auto pos2 = Position(File::A, Rank::FOUR);
    auto move = Move(pos1, pos2, board::PieceType::PAWN);
    chessboard.do_move(move);
    EXPECT_FALSE(chessboard.getBoardRpr().at(pos1).has_value());
    EXPECT_EQ(chessboard.getBoardRpr().at(pos2).value(), std::pair(PieceType::PAWN, Color::WHITE));
    chessboard.undo_move(move);
    EXPECT_TRUE(chessboard.getBoardRpr().at(pos1).has_value());
    EXPECT_FALSE(chessboard.getBoardRpr().at(pos2).has_value());
}

TEST(chessboard, do_move_undo_capture)
{
    using namespace board;
    auto chessboard = Chessboard();
    auto pos1 = Position(File::A, Rank::TWO);
    auto pos2 = Position(File::A, Rank::EIGHT);
    auto move = Move(pos1, pos2, board::PieceType::PAWN);
    move.setCapture(PieceType::ROOK);
    chessboard.do_move(move);
    EXPECT_FALSE(chessboard.getBoardRpr().at(pos1).has_value());
    EXPECT_EQ(chessboard.getBoardRpr().at(pos2).value(), std::pair(PieceType::PAWN, Color::WHITE));
    chessboard.getBoardRpr().print();
    chessboard.undo_move(move);
    chessboard.getBoardRpr().print();
    EXPECT_TRUE(chessboard.getBoardRpr().at(pos1).has_value());
    EXPECT_EQ(chessboard.getBoardRpr().at(pos2).value(), std::pair(PieceType::ROOK, Color::BLACK));
}


TEST(chessboard, do_move_undo_capture_promotion)
{
    using namespace board;
    auto chessboard = Chessboard();
    auto pos1 = Position(File::A, Rank::TWO);
    auto pos2 = Position(File::A, Rank::EIGHT);
    auto move = Move(pos1, pos2, board::PieceType::PAWN);
    move.setCapture(PieceType::ROOK);
    move.setPromotion(PieceType::QUEEN);
    chessboard.do_move(move);
    EXPECT_FALSE(chessboard.getBoardRpr().at(pos1).has_value());
    EXPECT_EQ(chessboard.getBoardRpr().at(pos2).value(), std::pair(PieceType::QUEEN, Color::WHITE));
    chessboard.undo_move(move);
    chessboard.getBoardRpr().print();
    EXPECT_EQ(chessboard.getBoardRpr().at(pos1).value(), std::pair(PieceType::PAWN, Color::WHITE));
    EXPECT_EQ(chessboard.getBoardRpr().at(pos2).value(), std::pair(PieceType::ROOK, Color::BLACK));
}

TEST(chessboard, is_move_valid_false)
{
    board::Masks::init();
    board::BitboardOperations::init_ms1bTable();
    board::magic::build_table();
    using namespace board;
    auto chessboard = Chessboard();
    auto pos1 = Position(File::A, Rank::TWO);
    auto pos2 = Position(File::A, Rank::EIGHT);
    auto move = Move(pos1, pos2, board::PieceType::PAWN);
    move.setCapture(PieceType::ROOK);
    move.setPromotion(PieceType::QUEEN);

    ASSERT_FALSE(chessboard.is_move_legal(move));
}

TEST(chessboard, is_move_valid_true)
{
    board::Masks::init();
    board::BitboardOperations::init_ms1bTable();
    board::magic::build_table();
    using namespace board;
    auto chessboard = Chessboard();
    auto pos1 = Position(File::A, Rank::TWO);
    auto pos2 = Position(File::A, Rank::FOUR);
    auto move = Move(pos1, pos2, board::PieceType::PAWN);

    ASSERT_TRUE(chessboard.is_move_legal(move));
}

TEST(chessboard, is_check_true)
{
    board::Masks::init();
    board::BitboardOperations::init_ms1bTable();
    board::magic::build_table();
    using namespace board;
    auto chessboard = Chessboard("");
    chessboard.setWhiteTurn(false);

    ASSERT_TRUE(chessboard.is_check());
}

TEST(chessboard, is_check_false)
{
    board::Masks::init();
    board::BitboardOperations::init_ms1bTable();
    board::magic::build_table();
    using namespace board;
    auto chessboard = Chessboard();
    chessboard.setWhiteTurn(false);

    ASSERT_FALSE(chessboard.is_check());
}

TEST(chessboard, castling_test_white_king_side)
{
    board::Masks::init();
    board::BitboardOperations::init_ms1bTable();
    board::magic::build_table();
    using namespace board;
    auto chessboard = Chessboard();

    auto &rpr = chessboard.getBoardRpr();
    rpr.boards[(int)PieceType::KNIGHT] = 0UL;
    rpr.boards[(int)PieceType::KNIGHT + 6] = 0UL;
    rpr.boards[(int)PieceType::QUEEN] = 0UL;
    rpr.boards[(int)PieceType::QUEEN + 6] = 0UL;
    rpr.boards[(int)PieceType::BISHOP] = 0UL;
    rpr.boards[(int)PieceType::BISHOP + 6] = 0UL;

    auto moves = chessboard.generate_legal_moves();

    chessboard.getBoardRpr().print();

    for (auto move : moves)
    {
        if (move.isKingCastling())
        {
            chessboard.do_move(move);
        }
    }

    chessboard.getBoardRpr().print();
    EXPECT_EQ(rpr.at(Position(5)).value(), (std::pair<PieceType, Color> (PieceType::ROOK, Color::WHITE)));
    EXPECT_EQ(rpr.at(Position(6)).value(), (std::pair<PieceType, Color> (PieceType::KING, Color::WHITE)));
}