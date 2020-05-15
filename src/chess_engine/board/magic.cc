//
// Created by mathieut on 5/15/20.
//

#include "magic.hh"
#include "masks.hh"

namespace board::magic
{
    BitBoard generate_blockboard(unsigned int index, BitBoard attackMask)
    {
        /* Start with a blockboard identical to the attackMask. */
        uint64_t blockboard = attackMask;

        /* Loop through the attackMask to find the indices of all set bits. */
        uint8_t bitindex = 0;
        for (uint8_t i = 0UL; i<64; i++) {
            /* Check if the i'th bit is set in the mask (and thus a potential blocker). */
            if ( attackMask & (1UL<<i) ) {
                /* Clear the i'th bit in the blockboard if it's clear in the index at bitindex. */
                if ( !(index & (1<<bitindex)) ) {
                    blockboard &= ~(1UL<<i); //Clear the bit.
                }
                /* Increment the bit index in the 0-4096 index, so each bit in index will correspond
                 * to each set bit in attackMask. */
                bitindex++;
            }
        }
        return blockboard;
    }

    BitBoard generate_attack(int index, BitBoard blockboard)
    {
        BitBoard attacks = 0ULL;

        // North West
        attacks |= Masks::bishop_attack_rays[Masks::NORTH_WEST][index];
        if (Masks::bishop_attack_rays[Masks::NORTH_WEST][index] & blockboard) {
            int blockerIndex = BitboardOperations::bitScanForward(Masks::bishop_attack_rays[Masks::Masks::NORTH_WEST][index] & blockboard);
            attacks &= ~Masks::bishop_attack_rays[Masks::NORTH_WEST][blockerIndex];
        }

        // North East
        attacks |= Masks::bishop_attack_rays[Masks::NORTH_EAST][index];
        if (Masks::bishop_attack_rays[Masks::NORTH_EAST][index] & blockboard) {
            int blockerIndex = BitboardOperations::bitScanForward(Masks::bishop_attack_rays[Masks::NORTH_EAST][index] & blockboard);
            attacks &= ~Masks::bishop_attack_rays[Masks::NORTH_EAST][blockerIndex];
        }

        // South East
        attacks |= Masks::bishop_attack_rays[Masks::SOUTH_EAST][index];
        if (Masks::bishop_attack_rays[Masks::SOUTH_EAST][index] & blockboard) {
            int blockerIndex = BitboardOperations::bitScanReverse(Masks::bishop_attack_rays[Masks::SOUTH_EAST][index] & blockboard);
            attacks &= ~Masks::bishop_attack_rays[Masks::SOUTH_EAST][blockerIndex];
        }

        // South West
        attacks |= Masks::bishop_attack_rays[Masks::SOUTH_WEST][index];
        if (Masks::bishop_attack_rays[Masks::SOUTH_WEST][index] & blockboard) {
            int blockerIndex = BitboardOperations::bitScanReverse(Masks::bishop_attack_rays[Masks::SOUTH_WEST][index] & blockboard);
            attacks &= ~Masks::bishop_attack_rays[Masks::SOUTH_WEST][blockerIndex];
        }

        return attacks;
    }
}