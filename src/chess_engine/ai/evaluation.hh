#pragma once

#include "../board/chessboard.hh"
namespace ai
{
    using namespace board;
    class Evaluation {
    public:
        Chessboard &chessboard_;

        int WdoubledPawns;
        int WblockedPawns;
        int WisolatedPawns;
        int BdoubledPawns;
        int BblockedPawns;
        int BisolatedPawns;

        bool iAmCheckmated;
        bool opponentIsCheckmated;

        explicit Evaluation(Chessboard &chessboard);
        float rate_chessboard(board::Color side);
        int count_pieces(PieceType pieceType, Color color);
        int count_pawns(Color color);
        bool is_pawn_blocked(uint8_t pawnSquare, Color color);
    };
}



