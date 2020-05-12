#pragma once

#include <utility>

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
        opt_piecetype_t promotion_;
        bool capture_;
        bool double_pawn_push_;
        bool king_castling_;
        bool queen_castling_;
        bool en_passant_;

    public:
        Move(Position start, Position dest, PieceType pieceType)
        : start_pos_(start)
        , dest_pos_(dest)
        , piece_(pieceType)
        {}

         Move(Position start, Position dest, PieceType pieceType,
              opt_piecetype_t promotion, bool capture)
        : start_pos_(start)
        , dest_pos_(dest)
        , piece_(pieceType)
        , promotion_(std::move(promotion))
        , capture_(capture)
        {}

        PieceType piece_get();
        Position start_pos_get();
        Position dest_pos_get();
        opt_piecetype_t get_promotion();
    };
} // namespace board
