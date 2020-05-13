#include <iostream>
#include "masks.hh"
#include "bitboard-operations.hh"

namespace board
{
    BitBoard Masks::king_attack[64];
    BitBoard Masks::knight_attack[64];
    BitBoard Masks::pawn_attack[2][64];

    BitBoard Masks::king_attacks(int position)
    {
        if (position < 64 && position >= 0)
            return king_attack[position];
        else
            throw std::range_error("Position must be lower than 64");
    }

    BitBoard computeKingAttack(BitBoard kingSet) {
        using bo = BitboardOperations;
        BitBoard attacks = bo::eastOne(kingSet) | bo::westOne(kingSet);
        kingSet    |= attacks;
        attacks    |= bo::nortOne(kingSet) | bo::soutOne(kingSet);
        return attacks;
    }

    BitBoard computeKnightAttack(BitBoard knightSet)
    {
        using bo = BitboardOperations;
        BitBoard west, east, attacks;
        east     = bo::eastOne (knightSet);
        west     = bo::westOne (knightSet);
        attacks  = (east|west) << 16;
        attacks |= (east|west) >> 16;
        east     = bo::eastOne (east);
        west     = bo::westOne (west);
        attacks |= (east|west) <<  8;
        attacks |= (east|west) >>  8;
        return attacks;
    }

    void Masks::computeKingAttacks()
    {
        unsigned long long pos = 1UL;
        for (size_t i = 0; i < 64; i++, pos <<= 1)
        {
            king_attack[i] = computeKingAttack(pos);
        }
    }

    void Masks::init() {
        computeKingAttacks();
        computeKnightAttacks();
        computePawnAttacks();
    }

    BitBoard Masks::knight_attacks(unsigned int i) {
        return knight_attack[i];
    }

    void Masks::computeKnightAttacks() {
        unsigned long long pos = 1UL;
        for (size_t i = 0; i < 64; i++, pos <<= 1)
        {
            knight_attack[i] = computeKnightAttack(pos);
        }
    }

    void Masks::computePawnAttacks() {
        using bo = BitboardOperations;
        unsigned long long pos = 1UL;
        for (int i = 0; i < 64; i++, pos <<= 1)
        {
            pawn_attack[0][i] = bo::noEaOne(pos) | bo::noWeOne(pos);
            pawn_attack[1][i] = bo::soEaOne(pos) | bo::soWeOne(pos);
            std::cout << '(' << i << ")\n";
            Chessboard_rpr::bitBoardPrint(pawn_attacks(Color::WHITE, i));
        }
    }

    BitBoard Masks::pawn_attacks(Color color, int i) {
        return pawn_attack[static_cast<int>(color)][i];
    }
}
