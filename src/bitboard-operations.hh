#pragma once


#include <cstdint>

namespace board
{
    using BitBoard = uint64_t;

    class BitboardOperations {
    private:
        static const BitBoard notHFile = 0x7f7f7f7f7f7f7f7f;
        static const BitBoard notAFile = 0xfefefefefefefefe;
    public:
        static BitBoard eastOne(BitBoard b);
        static BitBoard nortOne(BitBoard b);
        static BitBoard soutOne(BitBoard b);
        static BitBoard westOne(BitBoard b);
    };
}


