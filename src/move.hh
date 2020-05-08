#pragma once

#include "piece-type.hh"
#include "position.hh"

namespace board
{
    class Move
    {
    private:
        Position start_pos_;
        Position dest_pos_;
        PieceType piece_;
        PieceType promotion_;
        PieceType capture_;
        bool double_pawn_push_;
        bool king_castling_;
        bool queen_castling_;
        bool en_passant_;

    public:
        Move(Position start, Position dest, PieceType pieceType);
        PieceType piece_get();
        Position start_pos_get();
        Position dest_pos_get();
    };
} // namespace board
