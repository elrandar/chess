#pragma once

#include "../board/chessboard.hh"
namespace ai
{
    using namespace board;
    class Evaluation {
    public:
        Chessboard &chessboard_;
        std::vector<Move> whiteMoves;
        std::vector<Move> blackMoves;

        int WdoubledPawns;
        int WblockedPawns;
        int WisolatedPawns;
        int BdoubledPawns;
        int BblockedPawns;
        int BisolatedPawns;

        explicit Evaluation(Chessboard &chessboard);
        float rate_chessboard(board::Color side);
        int count_pieces(PieceType pieceType, Color color);
        int count_pawns(Color color);
        bool is_pawn_blocked(uint8_t pawnSquare, Color color);
    };
}



