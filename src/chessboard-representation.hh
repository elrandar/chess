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
        std::vector<BitBoard> boards;
    public:
        Chessboard_rpr();
        Chessboard_rpr(Move move);
        void alter_rpr(Move move);
        void print();
        std::optional<std::pair<PieceType, Color>> at(Position pos);
    };
}