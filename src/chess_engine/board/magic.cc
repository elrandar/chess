
#include "magic.hh"

#include <bitset>
#include <iostream>

#include "masks.hh"

namespace board::magic
{
    BitBoard BishopAttacks[0x12C0];
    BitBoard RookAttacks[0x16200];
    BitBoard* RookAttacksSquare[64];
    BitBoard* BishopAttacksSquare[64];

    void build_table_square(bool isARook, int sq)
    {
        BitBoard* Masks = isARook ? Masks::rook_attack : Masks::bishop_attack;
        const BitBoard* Magics = isARook ? RMagics : BMagics;
        BitBoard** AttacksSquare =
            isARook ? RookAttacksSquare : BishopAttacksSquare;
        BitBoard (*generate_attack)(int, BitBoard) =
            isARook ? generate_attack_rook : generate_attack_bishop;
        const unsigned* Shift = isARook ? RShift : BShift;

        const size_t tablesize = 1UL << (64 - Shift[sq]);
        std::bitset<64> bigbit = Masks[sq];
        auto nb_bbit = bigbit.count();
        for (int i = 0; i < (1 << nb_bbit); i++)
        {
            BitBoard blockboard = generate_blockboard(i, Masks[sq]);
            unsigned index = (blockboard * Magics[sq]) >> Shift[sq];
            AttacksSquare[sq][index] = generate_attack(sq, blockboard);
        }
        if (sq < 63)
            AttacksSquare[sq + 1] = AttacksSquare[sq] + tablesize;
    }

    void build_table()
    {
        RookAttacksSquare[0] = RookAttacks;
        BishopAttacksSquare[0] = BishopAttacks;

        for (int i = 0; i < 64; i++)
        {
            build_table_square(true, i);
            build_table_square(false, i);
        }
    }

    BitBoard generate_blockboard(int index, BitBoard attackMask)
    {
        /* Start with a blockboard identical to the attackMask. */
        BitBoard blockboard = attackMask;

        /* Loop through the attackMask to find the indices of all set bits. */
        unsigned bitindex = 0;
        for (unsigned i = 0UL; i < 64; i++)
        {
            /* Check if the i'th bit is set in the mask (and thus a potential
             * blocker). */
            if (attackMask & (1UL << i))
            {
                /* Clear the i'th bit in the blockboard if it's clear in the
                 * index at bitindex. */
                if (!(index & (1 << bitindex)))
                {
                    blockboard &= ~(1UL << i); // Clear the bit.
                }
                /* Increment the bit index in the 0-4096 index, so each bit in
                 * index will correspond to each set bit in attackMask. */
                bitindex++;
            }
        }
        return blockboard;
    }

    BitBoard attack_rook(BitBoard occ, int sq)
    {
        auto mask = Masks::rook_attacks(sq);
        auto magic = magic::RMagics[sq];
        auto shift = magic::RShift[sq];

        auto index = ((occ & mask) * magic) >> shift;

        return magic::RookAttacksSquare[sq][index];
    }

    BitBoard attack_bishop(BitBoard occ, int sq)
    {
        auto mask = Masks::bishop_attacks(sq);
        auto magic = magic::BMagics[sq];
        auto shift = magic::BShift[sq];

        auto index = ((occ & mask) * magic) >> shift;

        return magic::BishopAttacksSquare[sq][index];
    }

    BitBoard generate_attack_bishop(int index, BitBoard blockboard)
    {
        BitBoard attacks = 0ULL;

        // North West
        attacks |= Masks::bishop_attack_rays[Masks::NORTH_WEST][index];
        if (Masks::bishop_attack_rays[Masks::NORTH_WEST][index] & blockboard)
        {
            int blockerIndex = BitboardOperations::bitScanForward(
                Masks::bishop_attack_rays[Masks::Masks::NORTH_WEST][index]
                & blockboard);
            attacks &=
                ~Masks::bishop_attack_rays[Masks::NORTH_WEST][blockerIndex];
        }

        // North East
        attacks |= Masks::bishop_attack_rays[Masks::NORTH_EAST][index];
        if (Masks::bishop_attack_rays[Masks::NORTH_EAST][index] & blockboard)
        {
            int blockerIndex = BitboardOperations::bitScanForward(
                Masks::bishop_attack_rays[Masks::NORTH_EAST][index]
                & blockboard);
            attacks &=
                ~Masks::bishop_attack_rays[Masks::NORTH_EAST][blockerIndex];
        }

        // South East
        attacks |= Masks::bishop_attack_rays[Masks::SOUTH_EAST][index];
        if (Masks::bishop_attack_rays[Masks::SOUTH_EAST][index] & blockboard)
        {
            int blockerIndex = BitboardOperations::bitScanReverse(
                Masks::bishop_attack_rays[Masks::SOUTH_EAST][index]
                & blockboard);
            attacks &=
                ~Masks::bishop_attack_rays[Masks::SOUTH_EAST][blockerIndex];
        }

        // South West
        attacks |= Masks::bishop_attack_rays[Masks::SOUTH_WEST][index];
        if (Masks::bishop_attack_rays[Masks::SOUTH_WEST][index] & blockboard)
        {
            int blockerIndex = BitboardOperations::bitScanReverse(
                Masks::bishop_attack_rays[Masks::SOUTH_WEST][index]
                & blockboard);
            attacks &=
                ~Masks::bishop_attack_rays[Masks::SOUTH_WEST][blockerIndex];
        }

        return attacks;
    }

    BitBoard generate_attack_rook(int index, BitBoard blockboard)
    {
        BitBoard attacks = 0ULL;

        // North
        attacks |= Masks::rook_attack_rays[Masks::NORTH][index];
        if (Masks::rook_attack_rays[Masks::NORTH][index] & blockboard)
        {
            int blockerIndex = BitboardOperations::bitScanForward(
                Masks::rook_attack_rays[Masks::Masks::NORTH][index]
                & blockboard);
            attacks &= ~Masks::rook_attack_rays[Masks::NORTH][blockerIndex];
        }

        // WEST
        attacks |= Masks::rook_attack_rays[Masks::WEST][index];
        if (Masks::rook_attack_rays[Masks::WEST][index] & blockboard)
        {
            int blockerIndex = BitboardOperations::bitScanReverse(
                Masks::rook_attack_rays[Masks::WEST][index] & blockboard);
            attacks &= ~Masks::rook_attack_rays[Masks::WEST][blockerIndex];
        }

        // East
        attacks |= Masks::rook_attack_rays[Masks::EAST][index];
        if (Masks::rook_attack_rays[Masks::EAST][index] & blockboard)
        {
            int blockerIndex = BitboardOperations::bitScanForward(
                Masks::rook_attack_rays[Masks::EAST][index] & blockboard);
            attacks &= ~Masks::rook_attack_rays[Masks::EAST][blockerIndex];
        }

        // South
        attacks |= Masks::rook_attack_rays[Masks::SOUTH][index];
        if (Masks::rook_attack_rays[Masks::SOUTH][index] & blockboard)
        {
            int blockerIndex = BitboardOperations::bitScanReverse(
                Masks::rook_attack_rays[Masks::SOUTH][index] & blockboard);
            attacks &= ~Masks::rook_attack_rays[Masks::SOUTH][blockerIndex];
        }

        return attacks;
    }
} // namespace board::magic