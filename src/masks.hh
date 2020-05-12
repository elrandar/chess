#pragma once


#include "chessboard-representation.hh"

namespace board
{
    class Masks {
    private:
        static BitBoard king_attack[64];
        static void computeKingAttacks();
    public:
        static BitBoard king_attacks(int position);
        static void init();
    };


}


