#include "move.hh"

namespace board
{
    PieceType Move::piece_get()
    {
        return piece_;
    }
    Position Move::start_pos_get()
    {
        return start_pos_;
    }
    Position Move::dest_pos_get()
    {
        return dest_pos_;
    }

    Move::Move(Position start, Position dest, PieceType pieceType)
        : start_pos_(start)
        , dest_pos_(dest)
        , piece_(pieceType)
    {
        double_pawn_push_ = false;
        king_castling_ = false;
        queen_castling_ = false;
        en_passant_ = false;
        promotion_ = std::nullopt;
        capture_ = std::nullopt;
    }
    opt_piecetype_t Move::get_promotion()
    {
        return promotion_;
    }

    Move::Move(Position start, Position dest, PieceType pieceType, PieceType capture)
        : Move(start, dest, pieceType)
    {
        capture_ = capture;
    }

    Move::Move(Position start, Position end, PieceType pieceType, std::optional<PieceType> promotion, bool b)
        : Move(start, end, pieceType)
    {
        promotion_ = promotion;
        if (b)
        {
            capture_b_ = b;
        } else
        {
            capture_ = std::nullopt;
        }
    }

}