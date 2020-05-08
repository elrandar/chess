#pragma once

#include <cstdint>
#include "move.hh"

namespace board
{
    using BitBoard = uint64_t;

    class Chessboard_rpr
    {
    private:
        BitBoard white_king;
        BitBoard white_queen;
        BitBoard white_rook;
        BitBoard white_bishop;
        BitBoard white_pawn;
        BitBoard white_knight;

        BitBoard black_king;
        BitBoard black_queen;
        BitBoard black_rook;
        BitBoard black_bishop;
        BitBoard black_pawn;
        BitBoard black_knight;
    public:
        Chessboard_rpr();
        Chessboard_rpr(Move move);
        void alter_rpr(Move move);
        void print();
    };
}