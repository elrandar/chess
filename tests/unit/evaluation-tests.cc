
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

    std::vector<int> results = {1, 2, 2, 2, 8, 1};

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            EXPECT_EQ(evaluation.count_pieces(static_cast<PieceType>(i),
                    static_cast<Color>(j)), results.at(i));
        }
    }
}

TEST_F(evaluation, count_pawns_1)
{
    using namespace board;
    using namespace ai;
    auto board = Chessboard(perft_parser::parse_fen(
            "8/p2p4/p2p4/8/8/8/PPPP1PPP/8 b KQkq - 2 2"));

    auto evaluation = Evaluation(board);


    auto thing = evaluation.count_pawns(board::Color::BLACK);

    EXPECT_EQ(thing, 4);
    EXPECT_EQ(evaluation.BisolatedPawns, 4);
    EXPECT_EQ(evaluation.BblockedPawns, 2);
    EXPECT_EQ(evaluation.BdoubledPawns, 2);
}

TEST_F(evaluation, count_pawns_2)
{
    using namespace board;
    using namespace ai;
    auto board = Chessboard(perft_parser::parse_fen(
            "8/8/8/3P4/8/P2P3P/3P4/8 w - - 0 1"));
    board.getBoardRpr().print();

    auto evaluation = Evaluation(board);


    auto nbPawns = evaluation.count_pawns(board::Color::WHITE);

    EXPECT_EQ(nbPawns, 5);
    EXPECT_EQ(evaluation.WisolatedPawns, 5);
    EXPECT_EQ(evaluation.WblockedPawns, 1);
    EXPECT_EQ(evaluation.WdoubledPawns, 2);
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
