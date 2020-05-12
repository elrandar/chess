
#include "chessboard.hh"

namespace board
{
    void Chessboard::do_move(Move move)
    {
        boardRpr.alter_rpr(move);
    }

//    bool is_move_legal(Move move)
//    {
//
//    }

    Chessboard::Chessboard()
    {
        white_turn_ = true;
        white_king_castling_ = false;
        white_queen_castling_ = false;
        black_king_castling_ = false;
        black_queen_castling_ = false;
        turn_ = 1;
        last_fifty_turn_ = 50;
        boardRpr = Chessboard_rpr();
    }


}