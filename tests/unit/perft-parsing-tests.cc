#include <gtest/gtest.h>

#include "../../src/chess_engine/board/chessboard.hh"
#include "../../src/parsing/perft-parser/perft-object.hh"

TEST(perft_parsing, parsing_default_board)
{
    auto object = perft_parser::parse_perft(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    auto vec = std::vector<char>({'K', 'Q', 'k', 'q'});

    for (int i = 0; i < 63; i++)
    {
        auto fenObj = object.fen_get();
        if (i >= 16 && i <= 47)
        {
            EXPECT_EQ(object.fen_get()[board::Position(i)], std::nullopt);
        } else
        {
            EXPECT_NE(object.fen_get()[board::Position(i)], std::nullopt);
            if (i > 47)
                EXPECT_EQ(fenObj[board::Position(i)]->second,
                          board::Color::BLACK);
            else
                EXPECT_EQ(fenObj[board::Position(i)]->second,
                          board::Color::WHITE);
        }
        EXPECT_EQ(vec, fenObj.castling_get());
        EXPECT_EQ(board::Color::WHITE, fenObj.side_to_move_get());
        EXPECT_EQ(fenObj.en_passant_target_get(), std::nullopt);
    }
}

TEST(perft_parsing, parsing_kiwipete)
{
    auto object = perft_parser::parse_perft(
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 1");
    auto vec = std::vector<char>({'K', 'Q', 'k', 'q'});

    auto fenObj = object.fen_get();
    EXPECT_EQ(vec, fenObj.castling_get());
    EXPECT_EQ(board::Color::BLACK, fenObj.side_to_move_get());
    EXPECT_EQ(fenObj.en_passant_target_get(), std::nullopt);
}

TEST(perft_parsing, parsing_en_passant_set)
{
    using namespace board;
    auto object = perft_parser::parse_perft(
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b - a6 1");

    EXPECT_TRUE(object.fen_get().en_passant_target_get().has_value());
    EXPECT_EQ(object.fen_get().en_passant_target_get().value(),
              board::Position(board::File::A, board::Rank::SIX));

    auto chessboard = Chessboard(object.fen_get());

    EXPECT_EQ(
        BitboardOperations::bitScanForward(chessboard.getEnPassant().top()),
        40);
}
