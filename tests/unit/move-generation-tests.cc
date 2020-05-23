#include <gtest/gtest.h>

#include "../../src/chess_engine/board/chessboard.hh"
#include "../../src/chess_engine/board/magic.hh"
#include "../../src/chess_engine/board/masks.hh"
#include "../../src/parsing/perft-parser/perft-object.hh"

class move_generation : public ::testing::Test
{
protected:
    static void SetUpTestSuite()
    {
        board::Masks::init();
        board::BitboardOperations::init_ms1bTable();
        board::magic::build_table();
    }
};

TEST_F(move_generation, pawn_attacks)
{
    using namespace board;
    auto fenObj =
        perft_parser::parse_fen("8/8/8/8/n1n2n1n/1P4P1/8/8 w - - 0 1");
    auto cb = Chessboard(fenObj);

    auto moves = cb.generate_legal_moves();

    EXPECT_EQ(moves.size(), 6);

    auto expectedMoves = std::vector<Move>();

    expectedMoves.emplace_back(Position(File::B, Rank::THREE),
                               Position(File::A, Rank::FOUR),
                               board::PieceType::PAWN, PieceType::KNIGHT);
    expectedMoves.emplace_back(Position(File::B, Rank::THREE),
                               Position(File::A, Rank::FOUR),
                               board::PieceType::PAWN, PieceType::KNIGHT);
    expectedMoves.emplace_back(Position(File::B, Rank::THREE),
                               Position(File::A, Rank::FOUR),
                               board::PieceType::PAWN, PieceType::KNIGHT);
    expectedMoves.emplace_back(Position(File::B, Rank::THREE),
                               Position(File::A, Rank::FOUR),
                               board::PieceType::PAWN, PieceType::KNIGHT);

    for (auto expected : expectedMoves)
    {
        bool found = false;
        for (auto legalMove : moves)
        {
            if (legalMove == expected)
            {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }
}

TEST_F(move_generation, pawn_en_passant)
{
    using namespace board;
    auto fenObj = perft_parser::parse_fen("8/8/8/PpP5/8/8/8/8 w - b6 0 1");
    auto cb = Chessboard(fenObj);

    auto moves = cb.generate_legal_moves();

    EXPECT_EQ(moves.size(), 4);

    auto expectedMoves = std::vector<Move>();

    expectedMoves.emplace_back(Position(File::A, Rank::FIVE),
                               Position(File::A, Rank::SIX),
                               board::PieceType::PAWN);
    expectedMoves.emplace_back(Position(File::C, Rank::FIVE),
                               Position(File::C, Rank::SIX),
                               board::PieceType::PAWN);
    expectedMoves.emplace_back(Position(File::A, Rank::FIVE),
                               Position(File::B, Rank::SIX),
                               board::PieceType::PAWN, PieceType::PAWN);
    expectedMoves.at(2).setEnPassant(true);
    expectedMoves.emplace_back(Position(File::C, Rank::FIVE),
                               Position(File::B, Rank::SIX),
                               board::PieceType::PAWN, PieceType::PAWN);
    expectedMoves.at(3).setEnPassant(true);

    for (auto expected : expectedMoves)
    {
        bool found = false;
        for (auto legalMove : moves)
        {
            if (legalMove == expected)
            {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }
}

TEST_F(move_generation, pawn_promotion)
{
    using namespace board;
    auto fenObj = perft_parser::parse_fen("8/1P3P2/8/8/8/8/8/8 w - - 0 1");
    auto cb = Chessboard(fenObj);

    auto moves = cb.generate_legal_moves();

    EXPECT_EQ(moves.size(), 8);

    auto expectedMoves = std::vector<Move>();

    expectedMoves.emplace_back(
        Position(File::B, Rank::SEVEN), Position(File::B, Rank::EIGHT),
        board::PieceType::PAWN, std::optional<PieceType>(PieceType::QUEEN));
    expectedMoves.emplace_back(
        Position(File::F, Rank::SEVEN), Position(File::F, Rank::EIGHT),
        board::PieceType::PAWN, std::optional<PieceType>(PieceType::QUEEN));

    for (auto expected : expectedMoves)
    {
        bool found = false;
        for (auto legalMove : moves)
        {
            if (legalMove == expected)
            {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }
}

TEST_F(move_generation, pawn_promotion_capture)
{
    using namespace board;
    auto fenObj = perft_parser::parse_fen("r3r3/1P3P2/8/8/8/8/8/8 w - - 0 1");
    auto cb = Chessboard(fenObj);

    auto moves = cb.generate_legal_moves();

    EXPECT_EQ(moves.size(), 16);

    auto expectedMoves = std::vector<Move>();

    expectedMoves.emplace_back(
        Position(File::B, Rank::SEVEN), Position(File::B, Rank::EIGHT),
        board::PieceType::PAWN, std::optional<PieceType>(PieceType::QUEEN));
    expectedMoves.emplace_back(
        Position(File::F, Rank::SEVEN), Position(File::F, Rank::EIGHT),
        board::PieceType::PAWN, std::optional<PieceType>(PieceType::QUEEN));
    expectedMoves.emplace_back(
        Position(File::B, Rank::SEVEN), Position(File::A, Rank::EIGHT),
        board::PieceType::PAWN, std::optional<PieceType>(PieceType::QUEEN));
    expectedMoves.at(2).setCapture(PieceType::ROOK);
    expectedMoves.emplace_back(
        Position(File::F, Rank::SEVEN), Position(File::E, Rank::EIGHT),
        board::PieceType::PAWN, std::optional<PieceType>(PieceType::QUEEN));
    expectedMoves.at(3).setCapture(PieceType::ROOK);

    for (auto expected : expectedMoves)
    {
        bool found = false;
        for (auto legalMove : moves)
        {
            if (legalMove == expected)
            {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }
}

TEST_F(move_generation, pawn_promotion_capture_black)
{
    using namespace board;
    auto fenObj = perft_parser::parse_fen("8/8/8/8/8/8/1p6/R7 b - - 0 1");
    auto cb = Chessboard(fenObj);

    auto moves = cb.generate_legal_moves();

    EXPECT_EQ(moves.size(), 8);

    auto expectedMoves = std::vector<Move>();

    expectedMoves.emplace_back(
        Position(File::B, Rank::TWO), Position(File::A, Rank::ONE),
        board::PieceType::PAWN, std::optional<PieceType>(PieceType::QUEEN));
    expectedMoves.at(0).setCapture(PieceType::ROOK);
    for (auto expected : expectedMoves)
    {
        bool found = false;
        for (auto legalMove : moves)
        {
            if (legalMove == expected)
            {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }
}
