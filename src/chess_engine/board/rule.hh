#pragma once

#include <vector>
#include "move.hh"
#include "chessboard.hh"

namespace board
{
    class Rule
    {
    public:
        static std::vector<Move> generate_pawn_moves(Chessboard board);
        static std::vector<Move> generate_bishop_moves(Chessboard board);
        static std::vector<Move> generate_rook_moves(Chessboard board);
        static std::vector<Move> generate_king_moves(Chessboard board);
        static std::vector<Move> generate_queen_moves(Chessboard board);
        static std::vector<Move> generate_knight_moves(Chessboard board);
    };
}