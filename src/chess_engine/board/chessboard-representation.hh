#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include "move.hh"
#include "color.hh"
#include "chess_engine/board/bitboard-operations.hh"

namespace board
{

    class Chessboard_rpr
    {
    private:
    public:
        std::array<BitBoard, 12> boards;
        BitBoard enPassant;
        BitBoard occupied();
        BitBoard WhitePieces();
        BitBoard BlackPieces();
        BitBoard get(PieceType pieceType, Color color);
        static std::string bitBoardPrint(BitBoard bitBoard);
        Chessboard_rpr();
        Chessboard_rpr(Move move);
        void execute_move(Move move);
        void print();
        std::optional<std::pair<PieceType, Color>> at(Position pos);
    };
}