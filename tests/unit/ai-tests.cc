#include "../../src/chess_engine/ai/ai.hh"
#include "../../src/chess_engine/board/magic.hh"
#include "../../src/chess_engine/board/masks.hh"
#include <gtest/gtest.h>

class ai_protocol : public ::testing::Test {
protected:
    static void SetUpTestSuite()
    {
        board::Masks::init();
        board::BitboardOperations::init_ms1bTable();
        board::magic::build_table();
    }
};


TEST_F(ai_protocol, test_parse_fen_moves)
{
    auto ia = ai::Ai();
    ia.update_board("position fen rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 moves e2e4 e7e5");
    ia.print_board();
}

TEST_F(ai_protocol, test_parse_fen)
{
    auto ia = ai::Ai();
    ia.update_board("position fen rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    ia.print_board();
}

TEST_F(ai_protocol, test_parse_startpos)
{
    auto ia = ai::Ai();
    ia.update_board("position startpos");
    ia.print_board();
}

TEST_F(ai_protocol, test_parse_startpos_moves)
{
    auto ia = ai::Ai();
    ia.update_board("position startpos moves e2e4 e7e5");
    ia.print_board();
}

TEST_F(ai_protocol, test_parse_fen_promotion)
{
    auto ia = ai::Ai();
    ia.update_board("position fen 8/4P3/8/8/8/8/8/8 w - - 0 1 moves e7e8q");
    ia.print_board();
}

TEST_F(ai_protocol, test_print_move_promotion)
{
    auto move = board::Move(52, 60);
    move.setPromotion(board::PieceType::QUEEN);
    EXPECT_EQ(move.toString(), "e7e8q");
    move.setPromotion(board::PieceType::KNIGHT);
    EXPECT_EQ(move.toString(), "e7e8n");
    move.setPromotion(board::PieceType::BISHOP);
    EXPECT_EQ(move.toString(), "e7e8b");
    move.setPromotion(board::PieceType::ROOK);
    EXPECT_EQ(move.toString(), "e7e8r");
}

TEST_F(ai_protocol, test_print_move)
{
    auto move = board::Move(52, 60);
    EXPECT_EQ(move.toString(), "e7e8");
}
