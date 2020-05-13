#pragma once


#include "chessboard-representation.hh"

namespace board
{
    class Masks {
    private:
        static BitBoard king_attack[64];
        static BitBoard knight_attack[64];
        static BitBoard rook_attack[64];
        static BitBoard pawn_attack[2][64];
        static void computeKingAttacks();
        static void computeKnightAttacks();
        static void computePawnAttacks();
        static void computeRookAttacks();
    public:
        static BitBoard king_attacks(int position);
        static BitBoard pawn_attacks(Color color, int i);
        static void init();

        static BitBoard knight_attacks(int i);
        static BitBoard rook_attacks(int i);


    };


}


