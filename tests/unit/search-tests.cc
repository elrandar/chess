#include <gtest/gtest.h>
#include "../../src/chess_engine/ai/search.hh"
#include "../../src/chess_engine/board/magic.hh"
#include "../../src/chess_engine/board/masks.hh"
#include "../../src/parsing/perft-parser/perft-object.hh"

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

    auto obj = perft_parser::parse_fen("k7/8/4N3/8/8/2p5/3R2K1/8 b - - 0 1");
    board::Chessboard cb = board::Chessboard(obj);
    cb.print();
    auto move = ai::search::findNextMove(cb, 2);

    ASSERT_EQ(move.toString(), "c3d2");
}

TEST_F(search, checkmate)
{

    auto obj = perft_parser::parse_fen("r1bqkbnr/1ppppppp/8/p7/1nB1P3/5Q2/PPPP1PPP/RNB1K1NR w - - 0 1");
    board::Chessboard cb = board::Chessboard(obj);
    cb.print();
    auto move = ai::search::findNextMove(cb, 3);

    ASSERT_EQ(move.toString(), "c4f7");
}

TEST_F(search, checkmate_black_side)
{

    auto obj = perft_parser::parse_fen("r1bqkbnr/1ppppppp/8/p7/1nB1P3/5Q2/PPPP1PPP/RNB1K1NR b - - 0 1");
    board::Chessboard cb = board::Chessboard(obj);
    cb.print();
    auto move = ai::search::findNextMove(cb, 3);

    ASSERT_EQ(move.toString(), "b4c2");
}

TEST_F(search, checkmate_black_side2)
{

    auto obj = perft_parser::parse_fen("r1bqkbnr/1ppppppp/2n5/p7/2B1P3/5Q2/PPPP1PPP/RNB1K1NR b KQkq - 0 1");
    board::Chessboard cb = board::Chessboard(obj);
    cb.print();
    auto move = ai::search::findNextMove(cb, 3);

    ASSERT_EQ(move.toString(), "g8f6");
}
