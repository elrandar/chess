#include "../../src/chess_engine/board/chessboard.hh"
#include "../../src/chess_engine/board/magic.hh"
#include "../../src/chess_engine/board/masks.hh"
#include <gtest/gtest.h>
#include "../../src/parsing/perft-parser/perft-object.hh"


class chessboard : public ::testing::Test {
protected:
    static void SetUpTestSuite()
    {
        board::Masks::init();
        board::BitboardOperations::init_ms1bTable();
        board::magic::build_table();
    }
};

TEST_F(chessboard, king_position_white)
{
    using namespace board;
    auto cb = Chessboard();

    ASSERT_EQ(cb.king_position(), Position(File::E, board::Rank::ONE));
}

TEST_F(chessboard, king_position_black)
{
    using namespace board;
    auto cb = Chessboard();
    cb.setWhiteTurn(false);

    ASSERT_EQ(cb.king_position(), Position(File::E, board::Rank::EIGHT));
}
TEST_F(chessboard, do_execute_move)
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

TEST_F(chessboard, do_move_capture)
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

TEST_F(chessboard, do_move_undo)
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

TEST_F(chessboard, do_move_undo_capture)
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


    chessboard.undo_move(move);

    EXPECT_TRUE(chessboard.getBoardRpr().at(pos1).has_value());
    EXPECT_EQ(chessboard.getBoardRpr().at(pos2).value(), std::pair(PieceType::ROOK, Color::BLACK));
}


TEST_F(chessboard, do_move_undo_capture_promotion)
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
    EXPECT_EQ(chessboard.getBoardRpr().at(pos1).value(), std::pair(PieceType::PAWN, Color::WHITE));
    EXPECT_EQ(chessboard.getBoardRpr().at(pos2).value(), std::pair(PieceType::ROOK, Color::BLACK));
}

TEST_F(chessboard, is_move_valid_false)
{
    using namespace board;
    auto chessboard = Chessboard();
    auto pos1 = Position(File::A, Rank::TWO);
    auto pos2 = Position(File::A, Rank::EIGHT);
    auto move = Move(pos1, pos2, board::PieceType::PAWN);
    move.setCapture(PieceType::ROOK);
    move.setPromotion(PieceType::QUEEN);

    ASSERT_FALSE(chessboard.is_move_legal(move));
}

TEST_F(chessboard, is_move_valid_true)
{
    using namespace board;
    auto chessboard = Chessboard();
    auto pos1 = Position(File::A, Rank::TWO);
    auto pos2 = Position(File::A, Rank::FOUR);
    auto move = Move(pos1, pos2, board::PieceType::PAWN);
    move.setDoublePawnPush(true);

    ASSERT_TRUE(chessboard.is_move_legal(move));
}

TEST_F(chessboard, is_check_true)
{
    using namespace board;
    auto chessboard = Chessboard(
            perft_parser::parse_fen(
                    "4r3/8/8/8/8/8/4K3/8 w KQkq - 0"));

    ASSERT_TRUE(chessboard.is_check());
}

TEST_F(chessboard, is_check_false)
{
    using namespace board;
    auto chessboard = Chessboard();
    chessboard.setWhiteTurn(false);

    ASSERT_FALSE(chessboard.is_check());
}

TEST_F(chessboard, castling_test_white_king_side)
{
    using namespace board;
    auto chessboard = Chessboard();

    auto &rpr = chessboard.getBoardRpr();
    rpr.boards[(int)PieceType::KNIGHT] = 0UL;
    rpr.boards[(int)PieceType::QUEEN] = 0UL;
    rpr.boards[(int)PieceType::BISHOP] = 0UL;

    auto moves = chessboard.generate_legal_moves();


    for (auto move : moves)
    {
        if (move.isKingCastling())
        {
            chessboard.do_move(move);
            break;
        }
    }

    EXPECT_EQ(rpr.at(Position(5)).value(), (std::pair<PieceType, Color> (PieceType::ROOK, Color::WHITE)));
    EXPECT_EQ(rpr.at(Position(6)).value(), (std::pair<PieceType, Color> (PieceType::KING, Color::WHITE)));
}

TEST_F(chessboard, castling_test_white_queen_side)
{
    using namespace board;
    auto chessboard = Chessboard();

    auto &rpr = chessboard.getBoardRpr();
    rpr.boards[(int)PieceType::KNIGHT] = 0UL;
    rpr.boards[(int)PieceType::QUEEN] = 0UL;
    rpr.boards[(int)PieceType::BISHOP] = 0UL;

    auto moves = chessboard.generate_legal_moves();


    for (auto move : moves)
    {
        if (move.isQueenCastling())
        {
            chessboard.do_move(move);
            break;
        }
    }

    EXPECT_EQ(rpr.at(Position(3)).value(), (std::pair<PieceType, Color> (PieceType::ROOK, Color::WHITE)));
    EXPECT_EQ(rpr.at(Position(2)).value(), (std::pair<PieceType, Color> (PieceType::KING, Color::WHITE)));
}


TEST_F(chessboard, castling_test_black_king_side)
{
    using namespace board;
    auto chessboard = Chessboard();

    chessboard.setWhiteTurn(false);
    auto &rpr = chessboard.getBoardRpr();
    rpr.boards[(int)PieceType::KNIGHT + 6] = 0UL;
    rpr.boards[(int)PieceType::QUEEN + 6] = 0UL;
    rpr.boards[(int)PieceType::BISHOP + 6] = 0UL;

    auto moves = chessboard.generate_legal_moves();


    for (auto move : moves)
    {
        if (move.isKingCastling())
        {
            chessboard.do_move(move);
            break;
        }
    }

    EXPECT_EQ(rpr.at(Position(61)).value(), (std::pair<PieceType, Color> (PieceType::ROOK, Color::BLACK)));
    EXPECT_EQ(rpr.at(Position(62)).value(), (std::pair<PieceType, Color> (PieceType::KING, Color::BLACK)));
}

TEST_F(chessboard, castling_test_black_queen_side)
{
    using namespace board;
    auto chessboard = Chessboard();
    chessboard.setWhiteTurn(false);
    auto &rpr = chessboard.getBoardRpr();
    rpr.boards[(int)PieceType::KNIGHT + 6] = 0UL;
    rpr.boards[(int)PieceType::QUEEN + 6] = 0UL;
    rpr.boards[(int)PieceType::BISHOP + 6] = 0UL;

    auto moves = chessboard.generate_legal_moves();


    Move castlingMove = Move(Position(0), Position(0), board::PieceType::KING);
    for (auto move : moves)
    {
        if (move.isQueenCastling())
        {
            chessboard.do_move(move);
            castlingMove = move;
            break;
        }
    }

    EXPECT_EQ(rpr.at(Position(59)).value(), (std::pair<PieceType, Color> (PieceType::ROOK, Color::BLACK)));
    EXPECT_EQ(rpr.at(Position(58)).value(), (std::pair<PieceType, Color> (PieceType::KING, Color::BLACK)));

    chessboard.undo_move(castlingMove);

    EXPECT_FALSE(rpr.at(Position(59)).has_value());
    EXPECT_FALSE(rpr.at(Position(58)).has_value());
    EXPECT_EQ(rpr.at(Position(56)).value(), (std::pair<PieceType, Color> (PieceType::ROOK, Color::BLACK)));
    EXPECT_EQ(rpr.at(Position(60)).value(), (std::pair<PieceType, Color> (PieceType::KING, Color::BLACK)));

}
