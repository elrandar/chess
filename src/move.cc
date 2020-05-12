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

    opt_piecetype_t Move::get_promotion()
    {
        return promotion_;
    }

}