#include "../../src/chess_engine/board/zobrist.hh"
#include <gtest/gtest.h>
#include "../../src/parsing/perft-parser/perft-object.hh"
#include "../../src/chess_engine/board/masks.hh"
#include "../../src/chess_engine/board/magic.hh"

class zobrist : public ::testing::Test {
protected:
    static void SetUpTestSuite()
    {
        board::Zobrist::initRandomArray();
        board::Masks::init();
        board::BitboardOperations::init_ms1bTable();
        board::magic::build_table();
    }
};


TEST_F(zobrist, hash_test)
{

    auto cb = board::Chessboard();
    cb.do_move(board::Move(8, 16, board::PieceType::PAWN));
    auto hash = board::Zobrist::hash(cb);
    auto fenobj = perft_parser::parse_fen("rnbqkbnr/pppppppp/8/8/8/P7/1PPPPPPP/RNBQKBNR b KkqQ - 0 1");
    auto hash2 = board::Zobrist::hash(board::Chessboard(fenobj));
    ASSERT_EQ(hash, hash2);
}

TEST_F(zobrist, hash_test_do_undo)
{
    using namespace board;
    using namespace perft_parser;

    auto cb = board::Chessboard();

    auto legalMove = cb.generate_legal_moves();

    std::cout << legalMove[0].toString();

    auto hash = board::Zobrist::hash(cb);
    auto hash2 = Zobrist::updateHashWithMove(hash, legalMove[0], cb);
    hash2 = Zobrist::updateHashWithMove(hash2, legalMove[0], cb);

    ASSERT_EQ(hash, hash2);
}


TEST_F(zobrist, hash_do_equal_new_cb2)
{
    using namespace board;
    using namespace perft_parser;

    auto fen2 = parse_fen("8/8/8/8/3r4/8/8/8 b - - 0 1");
    auto cb = board::Chessboard(fen2);
    auto hash = Zobrist::hash(cb);
    hash = Zobrist::updateHashWithMove(hash,
                                       Move(27, 19, board::PieceType::ROOK),
                                       cb);

    auto fen = parse_fen("8/8/8/8/8/3r4/8/8 w - - 0 1");
    auto hash2 = board::Zobrist::hash(Chessboard(fen));

    ASSERT_EQ(hash, hash2);
}
