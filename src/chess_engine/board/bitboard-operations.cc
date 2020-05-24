
#include "bitboard-operations.hh"

#include <strings.h>

namespace board
{
    int BitboardOperations::ms1bTable[256];

    BitBoard BitboardOperations::eastOne(BitBoard b)
    {
        return (b << 1) & notAFile;
    }

    BitBoard BitboardOperations::westOne(BitBoard b)
    {
        return (b >> 1) & notHFile;
    }

    BitBoard BitboardOperations::nortOne(BitBoard b)
    {
        return (b << 8);
    }

    BitBoard BitboardOperations::soutOne(BitBoard b)
    {
        return (b >> 8);
    }

    BitBoard BitboardOperations::noEaOne(BitBoard b)
    {
        return (b << 9) & notAFile;
    }
    BitBoard BitboardOperations::soWeOne(BitBoard b)
    {
        return (b >> 9) & notHFile;
    }
    BitBoard BitboardOperations::noWeOne(BitBoard b)
    {
        return (b << 7) & notHFile;
    }
    BitBoard BitboardOperations::soEaOne(BitBoard b)
    {
        return (b >> 7) & notAFile;
    }

    void BitboardOperations::init_ms1bTable()
    {
        int i;
        for (i = 0; i < 256; i++)
        {
            BitboardOperations::ms1bTable[i] =
                ((i > 127) ? 7
                           : (i > 63) ? 6
                                      : (i > 31) ? 5
                                                 : (i > 15)
                                 ? 4
                                 : (i > 7) ? 3 : (i > 3) ? 2 : (i > 1) ? 1 : 0);
        }
    }

    int BitboardOperations::bitScanReverse(BitBoard bb)
    {
        int result = 0;
        if (bb > 0xFFFFFFFF)
        {
            bb >>= 32;
            result = 32;
        }
        if (bb > 0xFFFF)
        {
            bb >>= 16;
            result += 16;
        }
        if (bb > 0xFF)
        {
            bb >>= 8;
            result += 8;
        }
        return result + BitboardOperations::ms1bTable[bb];
    }

    int BitboardOperations::bitScanForward(BitBoard bb)
    {
        return ffsll(bb) - 1;
    }

    BitBoard BitboardOperations::trim_edges(int i, BitBoard bb)
    {
        if (!(i <= 7))
            bb &= ~FirstRank;
        if (!(i >= 56))
            bb &= ~EightRank;
        if (!(i % 8 == 0))
            bb &= notAFile;
        if (!(i % 8 == 7))
            bb &= notHFile;
        return bb;
    }
} // namespace board
