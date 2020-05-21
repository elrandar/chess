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


TEST_F(ai_protocol, test_parse)
{
    auto ia = ai::Ai();
    ia.update_board("position startpos moves e2e4");

}