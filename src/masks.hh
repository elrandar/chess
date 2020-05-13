#pragma once


#include "chessboard-representation.hh"

namespace board
{
    class Masks {
    private:
        static BitBoard king_attack[64];
        static BitBoard knight_attack[64];
        static void computeKingAttacks();
        static void computeKnightAttacks();
    public:
        static BitBoard king_attacks(int position);
        static void init();

        static BitBoard knight_attacks(unsigned int i);
    };


}


