
#include "../../src/chess_engine/ai/evaluation.hh"
#include "../../src/chess_engine/board/masks.hh"
#include "../../src/chess_engine/board/magic.hh"
#include "../../src/parsing/perft-parser/perft-object.hh"
#include <gtest/gtest.h>


class evaluation : public ::testing::Test {
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

TEST_F(evaluation, end_game_true)
{

    using namespace board;
    using namespace ai;
    auto board = Chessboard(perft_parser::parse_fen(
            "2bqk3/5p1p/8/8/8/8/1P1P1PPP/3QK1N1 w - - 0 1"));
    auto evaluation = Evaluation(board);

    ASSERT_TRUE(evaluation.is_end_game());
}

TEST_F(evaluation, end_game_false)
{

    using namespace board;
    using namespace ai;
    auto board = Chessboard();
    auto evaluation = Evaluation(board);

    ASSERT_FALSE(evaluation.is_end_game());
}

TEST_F(evaluation, end_game_true2)
{

    using namespace board;
    using namespace ai;
    auto board = Chessboard(perft_parser::parse_fen(
            "1n1qk3/p1p1pppp/8/8/8/8/PPPPP2P/2BQK3 w - - 0 1"));
    auto evaluation = Evaluation(board);

    ASSERT_TRUE(evaluation.is_end_game());
}

TEST_F(evaluation, end_game_true3)
{

    using namespace board;
    using namespace ai;
    auto board = Chessboard(perft_parser::parse_fen(
            "3qk3/p1p1pppp/8/8/8/8/PPPPP2P/2BQK3 w - - 0 1"));
    auto evaluation = Evaluation(board);

    ASSERT_TRUE(evaluation.is_end_game());
}

TEST_F(evaluation, end_game_false2)
{

    using namespace board;
    using namespace ai;
    auto board = Chessboard(perft_parser::parse_fen(
            "rnbqkb1r/ppp1pppp/3p4/3nP3/3P4/5N2/PPP2PPP/RNBQKB1R b KQkq - 1 4"));
    auto evaluation = Evaluation(board);

    ASSERT_FALSE(evaluation.is_end_game());
}

TEST_F(evaluation, end_game_false3)
{

    using namespace board;
    using namespace ai;
    auto board = Chessboard(perft_parser::parse_fen(
            "3q4/ppp1pppp/8/4P3/3P4/8/PP3PPP/3Q3R b - - 1 4"));
    auto evaluation = Evaluation(board);

    ASSERT_FALSE(evaluation.is_end_game());
}


TEST_F(evaluation, end_game_true5)
{

    using namespace board;
    using namespace ai;
    auto board = Chessboard(perft_parser::parse_fen(
            "2kqb3/ppp1pppp/8/4P3/3P4/8/PP3PPP/2K5 b - - 1 4"));
    auto evaluation = Evaluation(board);

    ASSERT_TRUE(evaluation.is_end_game());
}

TEST_F(evaluation, end_game_true6)
{

    using namespace board;
    using namespace ai;
    auto board = Chessboard(perft_parser::parse_fen(
            "2k1b3/ppp1pppp/8/4P3/3P4/8/PP3PPP/2KQ4 b - - 1 4"));
    auto evaluation = Evaluation(board);

    ASSERT_TRUE(evaluation.is_end_game());
}
