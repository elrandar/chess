#include <gtest/gtest.h>
#include "../../src/chess_engine/ai/search.hh"
#include "../../src/chess_engine/board/magic.hh"
#include "../../src/chess_engine/board/masks.hh"

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

    board::Chessboard cb = board::Chessboard();
    auto tree = ai::search::build_node(cb, 2);

    std::cout << "lol\n";
}