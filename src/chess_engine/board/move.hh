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
        Move(Position start, Position end, PieceType pieceType, std::optional<PieceType> promotion, bool b);
        PieceType piece_get();
        Position start_pos_get();
        Position dest_pos_get();

        bool operator==(const Move &rhs) const;

        bool operator!=(const Move &rhs) const;

        void setCapture(const std::optional<PieceType> &capture);

        bool isCaptureB() const;

        const std::optional<PieceType> &getCapture() const;
        opt_piecetype_t get_promotion();
        void setPromotion(const std::optional<PieceType> &promotion);
    };
} // namespace board
