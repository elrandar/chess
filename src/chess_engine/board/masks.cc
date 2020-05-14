#include <iostream>
#include "masks.hh"
#include "chess_engine/board/bitboard-operations.hh"

namespace board
{
    BitBoard Masks::king_attack[64];
    BitBoard Masks::knight_attack[64];
    BitBoard Masks::rook_attack[64];
    BitBoard Masks::bishop_attack[64];
    BitBoard Masks::pawn_attack[2][64];

    void Masks::init() {
        computeKingAttacks();
        computeKnightAttacks();
        computePawnAttacks();
        computeRookAttacks();
        computeBishopAttacks();
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
            rook_attack[i] = ((rankMask << (i & 56UL))| (fileMask << (i & 7UL))) ^ pos;
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
            bishop_attack[i] = (diagMask | antiDiagMask) ^ pos;
            Chessboard_rpr::bitBoardPrint(bishop_attack[i]);
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
