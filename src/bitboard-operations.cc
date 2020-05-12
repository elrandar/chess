
#include "bitboard-operations.hh"


namespace board
{
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
}

