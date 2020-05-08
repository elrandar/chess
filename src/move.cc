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
    {}

}