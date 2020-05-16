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
        std::optional<PieceType> promotion_;
        std::optional<PieceType> capture_;
        bool double_pawn_push_;
        bool king_castling_;
        bool queen_castling_;
        bool en_passant_;
        bool capture_b_;

    public:
        Move(Position start, Position dest, PieceType pieceType);
        Move(Position start, Position dest, PieceType pieceType, PieceType capture);
        PieceType piece_get();
        Position start_pos_get();
        Position dest_pos_get();

        Move(Position start, Position end, PieceType pieceType, std::optional<PieceType> promotion, bool b);

        opt_piecetype_t get_promotion();
    };
} // namespace board
