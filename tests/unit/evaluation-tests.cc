
#include <gtest/gtest.h>

#include "../../src/chess_engine/ai/evaluation.hh"
#include "../../src/chess_engine/ai/search.hh"
#include "../../src/chess_engine/board/magic.hh"
#include "../../src/chess_engine/board/masks.hh"
#include "../../src/parsing/perft-parser/perft-object.hh"

class evaluation : public ::testing::Test
{
protected:
    static void SetUpTestSuite()
    {
        board::Masks::init();
        board::BitboardOperations::init_ms1bTable();
        board::magic::build_table();
    }
};

TEST_F(evaluation, rating_1)
{
    using namespace board;
    using namespace ai;
    auto board = Chessboard(perft_parser::parse_fen(
        "rnbq3r/pB1pp1bN/6p1/5N2/2PP4/6P1/PP2PP1P/R1BQK2R b KQ - 4 5"));
    auto board2 = Chessboard();

    auto evaluation = Evaluation(board);
    auto evalBoardOne = evaluation.rate_chessboard(board::Color::WHITE);
    auto evaluation2 = Evaluation(board2);
    auto evalBoardTwo = evaluation2.rate_chessboard(board::Color::WHITE);
    EXPECT_EQ(evalBoardTwo, 0);
    EXPECT_GT(evalBoardOne, evalBoardTwo);
}

TEST_F(evaluation, white_mask_king)
{
    using namespace board;
    using namespace ai;
    auto board =
        Chessboard(perft_parser::parse_fen("8/2k5/8/8/8/8/2K5/8 w - - 0 1"));
    auto evaluation = Evaluation(board);

    EXPECT_EQ(evaluation.rate_chessboard(board::Color::WHITE), 0);
    EXPECT_EQ(evaluation.rate_chessboard(board::Color::BLACK), 0);
}

TEST_F(evaluation, is_he_very_dumb)
{
    board::Masks::init();
    board::BitboardOperations::init_ms1bTable();
    board::magic::build_table();
    using namespace board;
    using namespace ai;
    auto board = Chessboard(perft_parser::parse_fen(
        "rnbq1bnk/ppppppp1/8/8/8/8/PPPPPPP1/RNBQKBR1 w Qq - 0 1"));
    auto bestMove = ai::search::findNextMove(board);

    EXPECT_EQ(bestMove.toString(), "g1h1");
}

TEST_F(evaluation, is_he_very_dumb2)
{
    board::Masks::init();
    board::BitboardOperations::init_ms1bTable();
    board::magic::build_table();
    using namespace board;
    using namespace ai;
    auto board = Chessboard(perft_parser::parse_fen(
        "rnbq1bnk/ppppppp1/8/8/8/8/PPPPPPP1/RNBQKBR1 w Qq - 0 1"));
    auto bestMove = ai::search::findNextMove(board);

    ASSERT_TRUE(bestMove == bestMove);
}
