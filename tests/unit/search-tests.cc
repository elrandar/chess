#include <gtest/gtest.h>
#include "../../src/chess_engine/ai/search.hh"
#include "../../src/chess_engine/board/magic.hh"
#include "../../src/chess_engine/board/masks.hh"
#include "../../src/parsing/perft-parser/perft-object.hh"
#include "../../src/chess_engine/ai/gtree.hh"

class search : public ::testing::Test {
protected:
    static void SetUpTestSuite()
    {
        board::Masks::init();
        board::BitboardOperations::init_ms1bTable();
        board::magic::build_table();
    }
};

TEST_F(search, eat_that_rook)
{
    board::Masks::init();
    board::BitboardOperations::init_ms1bTable();
    board::magic::build_table();

    auto obj = perft_parser::parse_fen("k7/8/4N3/8/8/2p5/3R2K1/8 b - - 0 1");
    board::Chessboard cb = board::Chessboard(obj);
    cb.print();
    auto move = ai::search::findNextMove(cb, 4);

    ASSERT_EQ(move.toString(), "c3d2");
}

