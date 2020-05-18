#pragma once

#include <vector>
#include "move.hh"
#include "chessboard.hh"

namespace board
{
    class Rule
    {
    private:
        static std::vector<Move>
        generate_pawn_moves_color(Chessboard_rpr &boardRpr, Color color, std::vector<Move> &moves, BitBoard en_passant);
        static std::vector<Move>
        generate_knight_king_moves_color(PieceType pieceType, Color color, Chessboard_rpr &boardRpr,
                                         std::vector<Move> &moves);
        static std::vector<Move>
        generate_bishop_rook_moves_color(Chessboard_rpr &chessboardRpr, Color color, PieceType pieceType,
                                         std::vector<Move> &moves, bool isQueen);
    public:
        static std::vector<Move> generate_pawn_moves(Chessboard &board);
        static std::vector<Move> generate_bishop_moves(Chessboard &board);
        static std::vector<Move> generate_rook_moves(Chessboard &board);
        static std::vector<Move> generate_king_moves(Chessboard &board);
        static std::vector<Move> generate_queen_moves(Chessboard &board);
        static std::vector<Move> generate_knight_moves(Chessboard &board);

    };
}