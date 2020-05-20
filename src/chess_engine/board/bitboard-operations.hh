#pragma once


#include <cstdint>

namespace board
{
    using BitBoard = uint64_t;

    class BitboardOperations {
    private:
        static const BitBoard notHFile = 0x7f7f7f7f7f7f7f7f;
        static const BitBoard notAFile = 0xfefefefefefefefe;
        static const BitBoard HFile = 0x8080808080808080;
        static const BitBoard AFile = 0x101010101010101;
        static const BitBoard FirstRank = 0xff;
        static const BitBoard EightRank = 0xff00000000000000;
    public:
        static const BitBoard rank4 = 0x00000000FF000000;
        static const BitBoard rank5 = 0x000000FF00000000;
        static BitBoard eastOne(BitBoard b);
        static BitBoard nortOne(BitBoard b);
        static BitBoard soutOne(BitBoard b);
        static BitBoard westOne(BitBoard b);
        static BitBoard noEaOne(BitBoard b);
        static BitBoard soWeOne(BitBoard b);
        static BitBoard noWeOne(BitBoard b);
        static BitBoard soEaOne(BitBoard b);
        static int bitScanReverse(BitBoard bb);
        static int bitScanForward(BitBoard bb);
        static BitBoard trim_edges(int i, BitBoard bb);
        static void init_ms1bTable();
        static int ms1bTable[256];

    };
}


