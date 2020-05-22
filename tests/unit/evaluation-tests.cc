
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

TEST_F(evaluation, count_pieces_1)
{
    using namespace board;
    using namespace ai;
    auto board = Chessboard();
    auto evaluation = Evaluation(board);


}


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
