#include <iostream>
#include <strings.h>
#include "masks.hh"
#include "chessboard-representation.hh"

namespace board
{
    BitBoard Masks::king_attack[64];
    BitBoard Masks::knight_attack[64];
    BitBoard Masks::rook_attack[64];
    BitBoard Masks::bishop_attack[64];
    BitBoard Masks::pawn_attack[2][64];
    BitBoard Masks::rook_attack_rays[4][64];
    BitBoard Masks::bishop_attack_rays[4][64];

    void Masks::init() {
        computeKingAttacks();
        computeKnightAttacks();
        computePawnAttacks();
        computeRookAttacks();
        computeRookRayAttacks();
        computeBishopAttacks();
        computeBishopRayAttacks();
    }


    //===========================KING================================
    BitBoard computeKingAttack(BitBoard kingSet) {
        using bo = BitboardOperations;
        BitBoard attacks = bo::eastOne(kingSet) | bo::westOne(kingSet);
        kingSet    |= attacks;
        attacks    |= bo::nortOne(kingSet) | bo::soutOne(kingSet);
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

    BitBoard Masks::king_attacks(int position)
    {
        if (position < 64 && position >= 0)
            return king_attack[position];
        else
            throw std::range_error("Position must be lower than 64");
    }

    //===========================KNIGHT================================

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


    void Masks::computeKnightAttacks() {
        unsigned long long pos = 1UL;
        for (size_t i = 0; i < 64; i++, pos <<= 1)
        {
            knight_attack[i] = computeKnightAttack(pos);
        }
    }

    BitBoard Masks::knight_attacks(int i) {
        return knight_attack[i];
    }

    //===========================ROOK================================

    void Masks::computeRookAttacks()
    {
        unsigned long long pos = 1UL;
        const BitBoard rankMask = 0xff;
        const BitBoard fileMask = 0x0101010101010101;
        for (size_t i = 0; i < 64; i++, pos <<= 1UL)
        {
            rook_attack[i] = (((rankMask << (i & 56UL))| (fileMask << (i & 7UL))) ^ pos) & 0x7e7e7e7e7e7e00UL;
        }
    }

    void Masks::computeRookRayAttacks()
    {
        for (size_t i = 0; i < 64; i++)
        {
            const BitBoard one = 1;
            rook_attack_rays[NORTH][i] = 0x0101010101010100 << i;
            rook_attack_rays[EAST][i] = 2*( (one << (i|7)) - (one << i) );
            rook_attack_rays[SOUTH][i] = 0x0080808080808080 >> (i ^ 63);
            rook_attack_rays[WEST][i] = (one << i) - (one << (i & 56));
        }
    }

    BitBoard Masks::rook_attacks(int i) {
        return rook_attack[i];
    }

    //===========================BISHOP================================

    void Masks::computeBishopAttacks()
    {
        unsigned long long pos = 1ul;
        const BitBoard mainDiagonal = 0x8040201008040201;
        const BitBoard mainAntiDiag = 0x0102040810204080;
        for (size_t i = 0; i < 64; i++, pos <<= 1UL)
        {
            int diag = 8*(i & 7) - (i & 56);
            int nort = -diag & ( diag >> 31);
            int sout =  diag & (-diag >> 31);
            BitBoard diagMask = (mainDiagonal >> sout) << nort;
            diag = 56 - 8 * (i & 7) - (i & 56);
            nort = -diag & ( diag >> 31);
            sout =  diag & (-diag >> 31);
            BitBoard antiDiagMask = (mainAntiDiag >> sout) << nort;
            bishop_attack[i] = ((diagMask | antiDiagMask) ^ pos) & 0x7e7e7e7e7e7e00UL;
        }
    }

    void Masks::computeBishopRayAttacks()
    {
        for (int i = 0; i < 64; i++)
        {
            const BitBoard maindia = 0x8040201008040201;
            int diag = 8*(i & 7) - (i & 56);
            int nort = -diag & ( diag >> 31);
            int sout =  diag & (-diag >> 31);
            BitBoard diagMask = (maindia >> sout) << nort;

            const BitBoard antimaindia = 0x0102040810204080;
            int antidiag = 56- 8*(i&7) - (i&56);
            int antinort = -antidiag & ( antidiag >> 31);
            int antisout =  antidiag & (-antidiag >> 31);
            BitBoard antiDiagMask = (antimaindia >> antisout) << antinort;

            BitBoard upperDiag = diagMask;
            while ( ffsll(upperDiag) - 1 != i)
            {
                unsigned lsb = ffsll(upperDiag) - 1;
                upperDiag &= ~(1UL << lsb);
            }
            BitBoard lowerDiag = (diagMask ^ upperDiag);
            upperDiag &= ~(1UL << i);

            BitBoard upperAntiDiag = antiDiagMask;
            while ( ffsll(upperAntiDiag) - 1 != i)
            {
                unsigned lsb = ffsll(upperAntiDiag) - 1;
                upperAntiDiag &= ~(1UL << lsb);
            }
            BitBoard lowerAntiDiag = (antiDiagMask ^ upperAntiDiag);
            upperAntiDiag &= ~(1UL << i);

            bishop_attack_rays[NORTH_EAST][i] = upperDiag;
            bishop_attack_rays[SOUTH_EAST][i] = lowerDiag;
            bishop_attack_rays[SOUTH_WEST][i] = lowerAntiDiag;
            bishop_attack_rays[NORTH_WEST][i] = upperAntiDiag;
        }
    }

    BitBoard Masks::bishop_attacks(int i) {
        return bishop_attack[i];
    }

    //===========================PAWN================================

    void Masks::computePawnAttacks() {
        using bo = BitboardOperations;
        unsigned long long pos = 1UL;
        for (int i = 0; i < 64; i++, pos <<= 1)
        {
            pawn_attack[0][i] = bo::noEaOne(pos) | bo::noWeOne(pos);
            pawn_attack[1][i] = bo::soEaOne(pos) | bo::soWeOne(pos);
        }
    }

    BitBoard Masks::pawn_attacks(Color color, int i) {
        return pawn_attack[static_cast<int>(color)][i];
    }

}
