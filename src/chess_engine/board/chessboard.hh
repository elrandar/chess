#pragma once

#include <vector>
#include "move.hh"
#include "color.hh"
#include "chessboard-representation.hh"

namespace board
{
    class Chessboard
    {
    private:
        Chessboard_rpr boardRpr;
        bool white_turn_;
        bool white_king_castling_;
        bool white_queen_castling_;
        bool black_king_castling_;
        bool black_queen_castling_;
        //TODO en_passant;
        unsigned int turn_;
        unsigned int last_fifty_turn_;

    public:
        Chessboard();
        std::vector<Move> generate_legal_moves();
        void do_move(Move move);
        bool is_move_legal(Move move);
        bool is_check();
        bool is_checkmate();
        bool is_draw();
        std::pair<PieceType, Color> operator[](Position position);
        Chessboard_rpr &getBoardRpr();

    };
}