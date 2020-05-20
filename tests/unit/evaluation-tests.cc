
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
    board.getBoardRpr().print();

    auto evaluation = Evaluation(board);


    auto thing = evaluation.count_pawns(board::Color::BLACK);

    EXPECT_EQ(thing, 4);

}
