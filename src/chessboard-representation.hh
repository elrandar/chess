#pragma once

#include <cstdint>
#include <vector>
#include "move.hh"
#include "color.hh"

namespace board
{
    using BitBoard = uint64_t;

    class Chessboard_rpr
    {
    private:
    public:
        std::vector<BitBoard> boards;
        BitBoard generalBoard();
        static void bitBoardPrint(BitBoard bitBoard);
        Chessboard_rpr();
        Chessboard_rpr(Move move);
        void execute_move(Move move);
        void print();
        std::optional<std::pair<PieceType, Color>> at(Position pos);
    };
}