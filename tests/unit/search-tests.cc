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

TEST_F(search, build_basic_tree)
{

    auto obj = perft_parser::parse_fen("k7/8/4N3/8/8/2p5/3R2K1/8 b - - 0 1");
    board::Chessboard cb = board::Chessboard(obj);
    auto tree = ai::search::findNextMove(cb);



//    board::Chessboard cb2 = board::Chessboard();
//    cb2.getMatchingLegalMoveAndDo(board::Move(9, 17));
//    auto tree2 = ai::search::findNextMove(cb2);

    std::cout << tree.toString() << " "; //<< tree2.toString();
}