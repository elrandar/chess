//
// Created by mathieut on 5/24/20.
//
#pragma once

#include "../board/color.hh"
#include "../board/chessboard-representation.hh"
#include "../board/color.hh"
#include "../board/piece-type.hh"
#include "../board/bitboard-operations.hh"


namespace ai
{
    class tools
    {
    public:
        static int nb_piece(board::PieceType pieceType, board::Color color, board::Chessboard_rpr &rpr);
        static int gamePhase(board::Chessboard_rpr &rpr);
        static int fpap(board::Color color, board::Chessboard_rpr& rpr, board::BitBoard fileToCheck);
};
}
