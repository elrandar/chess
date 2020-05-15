//
// Created by mathieut on 5/15/20.
//

#pragma once


#include "bitboard-operations.hh"

namespace board::magic {

        constexpr static unsigned RShift[64] = {
                52, 53, 53, 53, 53, 53, 53, 52,
                53, 54, 54, 53, 54, 54, 54, 53,
                53, 54, 54, 54, 54, 54, 54, 53,
                53, 54, 54, 54, 54, 54, 54, 53,
                53, 54, 54, 54, 54, 54, 54, 53,
                53, 54, 54, 53, 54, 54, 54, 53,
                54, 55, 55, 55, 55, 55, 54, 54,
                53, 54, 54, 54, 54, 53, 54, 53,
        };

        // Very dense and high quality magics taken from Texel
        constexpr static BitBoard RMagics[64] = {
                0x19A80065FF2BFFFFULL, 0x3FD80075FFEBFFFFULL, 0x4010000DF6F6FFFEULL,
                0x0050001FAFFAFFFFULL, 0x0050028004FFFFB0ULL, 0x7F600280089FFFF1ULL,
                0x7F5000B0029FFFFCULL, 0x5B58004848A7FFFAULL, 0x002A90005547FFFFULL,
                0x000050007F13FFFFULL, 0x007FA0006013FFFFULL, 0x006A9005656FFFFFULL,
                0x007F600F600AFFFFULL, 0x007EC007E6BFFFE2ULL, 0x007EC003EEBFFFFBULL,
                0x0071D002382FFFDAULL, 0x009F803000E7FFFAULL, 0x00680030008BFFFFULL,
                0x00606060004F3FFCULL, 0x001A00600BFF9FFDULL, 0x000D006005FF9FFFULL,
                0x0001806003005FFFULL, 0x00000300040BFFFAULL, 0x000192500065FFEAULL,
                0x00FFF112D0006800ULL, 0x007FF037D000C004ULL, 0x003FD062001A3FF8ULL,
                0x00087000600E1FFCULL, 0x000FFF0100100804ULL, 0x0007FF0100080402ULL,
                0x0003FFE0C0060003ULL, 0x0001FFD53000D300ULL, 0x00FFFD3000600061ULL,
                0x007FFF7F95900040ULL, 0x003FFF8C00600060ULL, 0x001FFE2587A01860ULL,
                0x000FFF3FBF40180CULL, 0x0007FFC73F400C06ULL, 0x0003FF86D2C01405ULL,
                0x0001FFFEAA700100ULL, 0x00FFFDFDD8005000ULL, 0x007FFF80EBFFB000ULL,
                0x003FFFDF603F6000ULL, 0x001FFFE050405000ULL, 0x000FFF400700C00CULL,
                0x0007FF6007BF600AULL, 0x0003FFEEBFFEC005ULL, 0x0001FFFDF3FEB001ULL,
                0x00FFFF39FF484A00ULL, 0x007FFF3FFF486300ULL, 0x003FFF99FFAC2E00ULL,
                0x001FFF31FF2A6A00ULL, 0x000FFF19FF15B600ULL, 0x0007FFF5FFF28600ULL,
                0x0003FFFDDFFBFEE0ULL, 0x0001FFF5F63C96A0ULL, 0x00FFFF5DFF65CFB6ULL,
                0x007FFFBAFFD1C5AEULL, 0x003FFF71FF6CBCEAULL, 0x001FFFD9FFD4756EULL,
                0x000FFFF5FFF338E6ULL, 0x0007FFFDFFFE24F6ULL, 0x0003FFEF27EEBE74ULL,
                0x0001FFFF23FF605EULL
        };

        constexpr static unsigned BShift[64] = {
                59, 60, 59, 59, 59, 59, 60, 59,
                60, 60, 59, 59, 59, 59, 60, 60,
                60, 60, 57, 57, 57, 57, 60, 60,
                59, 59, 57, 55, 55, 57, 59, 59,
                59, 59, 57, 55, 55, 57, 59, 59,
                60, 60, 57, 57, 57, 57, 60, 60,
                60, 60, 59, 59, 59, 59, 60, 60,
                59, 60, 59, 59, 59, 59, 60, 59,
        };

        constexpr static BitBoard BMagics[64] = {
                0x0006EFF5367FF600ULL, 0x00345835BA77FF2BULL, 0x00145F68A3F5DAB6ULL,
                0x003A1863FB56F21DULL, 0x0012EB6BFE9D93CDULL, 0x000D82827F3420D6ULL,
                0x00074BCD9C7FEC97ULL, 0x000034FE99F9FFFFULL, 0x0000746F8D6717F6ULL,
                0x00003ACB32E1A3F7ULL, 0x0000185DAF1FFB8AULL, 0x00003A1867F17067ULL,
                0x0000038EE0CCF92EULL, 0x000002A2B7FF926EULL, 0x000006C9AA93FF14ULL,
                0x00000399B5E5BF87ULL, 0x00400F342C951FFCULL, 0x0020230579ED8FF0ULL,
                0x007B008A0077DBFDULL, 0x001D00010C13FD46ULL, 0x00040022031C1FFBULL,
                0x000FA00FD1CBFF79ULL, 0x000400A4BC9AFFDFULL, 0x000200085E9CFFDAULL,
                0x002A14560A3DBFBDULL, 0x000A0A157B9EAFD1ULL, 0x00060600FD002FFAULL,
                0x004006000C009010ULL, 0x001A002042008040ULL, 0x001A00600FD1FFC0ULL,
                0x000D0ACE50BF3F8DULL, 0x000183A48434EFD1ULL, 0x001FBD7670982A0DULL,
                0x000FE24301D81A0FULL, 0x0007FBF82F040041ULL, 0x000040C800008200ULL,
                0x007FE17018086006ULL, 0x003B7DDF0FFE1EFFULL, 0x001F92F861DF4A0AULL,
                0x000FD713AD98A289ULL, 0x000FD6AA751E400CULL, 0x0007F2A63AE9600CULL,
                0x0003FF7DFE0E3F00ULL, 0x000003FD2704CE04ULL, 0x00007FC421601D40ULL,
                0x007FFF5F70900120ULL, 0x003FA66283556403ULL, 0x001FE31969AEC201ULL,
                0x0007FDFC18AC14BBULL, 0x0003FB96FB568A47ULL, 0x000003F72EA4954DULL,
                0x00000003F8DC0383ULL, 0x0000007F3A814490ULL, 0x00007DC5C9CF62A6ULL,
                0x007F23D3342897ACULL, 0x003FEE36EEE1565CULL, 0x0003FF3E99FCCCC7ULL,
                0x000003ECFCFAC5FEULL, 0x00000003F97F7453ULL, 0x0000000003F8DC03ULL,
                0x000000007EFA8146ULL, 0x0000007ED3E2EF60ULL, 0x00007F47243ADCD6ULL,
                0x007FB65AFABFB3B5ULL,
        };

        BitBoard generate_blockboard(int index, BitBoard attackMask);
        BitBoard generate_attack_bishop(int index, BitBoard blockboard);
        BitBoard generate_attack_rook(int index, BitBoard blockboard);

}
