#pragma once


#include "chess_engine/board/chessboard-representation.hh"

namespace board
{
    class Masks {
    private:
        static BitBoard king_attack[64];
        static BitBoard knight_attack[64];
        static BitBoard rook_attack[64];
        static BitBoard bishop_attack[64];
        static BitBoard pawn_attack[2][64];
        static void computeKingAttacks();
        static void computeKnightAttacks();
        static void computePawnAttacks();
        static void computeRookAttacks();
        static void computeBishopAttacks();
    public:
        static void init();
        static BitBoard king_attacks(int position);
        static BitBoard knight_attacks(int i);
        static BitBoard rook_attacks(int i);
        static BitBoard bishop_attacks(int i);
        static BitBoard pawn_attacks(Color color, int i);
    };
}


