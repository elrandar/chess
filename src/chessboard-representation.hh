#pragma once

#include <cstdint>
#include <vector>
#include "move.hh"
#include "color.hh"
#include "bitboard-operations.hh"

namespace board
{

    class Chessboard_rpr
    {
    private:
    public:
        std::vector<BitBoard> boards;
        BitBoard occupied();
        BitBoard WhitePieces();
        BitBoard BlackPieces();
        BitBoard get(PieceType pieceType, Color color);
        static void bitBoardPrint(BitBoard bitBoard);
        Chessboard_rpr();
        Chessboard_rpr(Move move);
        void execute_move(Move move);
        void print();
        std::optional<std::pair<PieceType, Color>> at(Position pos);
    };
}