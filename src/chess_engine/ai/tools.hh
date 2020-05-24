//
// Created by mathieut on 5/24/20.
//

#ifndef CHESS_TOOLS_HH
#define CHESS_TOOLS_HH

#include "../board/color.hh"
#include "../board/chessboard-representation.hh"
#include "../board/piece-type.hh"

namespace ai
{
    class tools {
    public:
        static int nb_piece(board::PieceType pieceType, board::Color color, board::Chessboard_rpr &rpr);
        static int gamePhase(board::Chessboard_rpr &rpr);

    };
}


#endif //CHESS_TOOLS_HH
