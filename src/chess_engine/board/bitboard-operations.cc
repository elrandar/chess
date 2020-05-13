
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
}

