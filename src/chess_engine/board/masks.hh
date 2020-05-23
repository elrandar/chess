#pragma once

#include "bitboard-operations.hh"
#include "color.hh"

namespace board
{
    class Masks
    {
    public:
        enum rook_direction
        {
            NORTH = 0,
            EAST,
            SOUTH,
            WEST
        };

        enum bishop_direction
        {
            NORTH_EAST = 0,
            SOUTH_EAST,
            SOUTH_WEST,
            NORTH_WEST

        };

        static BitBoard king_attack[64];
        static BitBoard knight_attack[64];
        static BitBoard pawn_attack[2][64];

        static BitBoard rook_attack[64];
        static BitBoard bishop_attack[64];

        // used for classical attack generation
        static BitBoard rook_attack_rays[4][64];
        static BitBoard bishop_attack_rays[4][64];

        static void computeKingAttacks();
        static void computeKnightAttacks();
        static void computePawnAttacks();

        static void computeRookAttacks();
        static void computeRookRayAttacks();

        static void computeBishopAttacks();
        static void computeBishopRayAttacks();

        static void init();
        static BitBoard king_attacks(int position);
        static BitBoard knight_attacks(int i);
        static BitBoard rook_attacks(int i);
        static BitBoard bishop_attacks(int i);
        static BitBoard pawn_attacks(Color color, int i);
    };
} // namespace board
