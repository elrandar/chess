#pragma once


#include <cstdint>
#include "chessboard.hh"

namespace board
{
    class Zobrist
    {
    private:

        static uint64_t randomArray[2][6][64];
        static uint64_t randomArray2[1 + 4 + 8];

    public:
        static void initRandomArray();

        static uint64_t hash(board::Chessboard cb);

        static uint64_t
        updateHashWithMove(uint64_t oldHash, Move move, Chessboard cb);
    };
}