
#include <gtest/gtest.h>
#include "../../src/chess_engine/board/magic.hh"
#include "../../src/chess_engine/board/masks.hh"


class bishop_generation : public ::testing::Test {
protected:
    bishop_generation()
    {
        board::Masks::init();
        board::BitboardOperations::init_ms1bTable();
    }
};

TEST_F(bishop_generation, bishop_blockboard_1)
{
    auto result = board::magic::generate_attack_bishop(27, 0x8041020000102041);
    ASSERT_EQ(result, (unsigned long) 0x40221400140201ul);
}

TEST_F(bishop_generation, bishop_blockboard_2)
{
    auto result = board::magic::generate_attack_bishop(50, 0x200020120000000);
    ASSERT_EQ(result, (unsigned long) 0xa000a1020000000);
}

TEST_F(bishop_generation, bishop_blockboard_3)
{
    auto result = board::magic::generate_attack_bishop(36, 0x4000000040001);
    ASSERT_EQ(result, (unsigned long) 0x8044280028448000);
}

TEST_F(bishop_generation, bishop_blockboard_4)
{
    auto result = board::magic::generate_attack_bishop(0, 0x0);
    ASSERT_EQ(result, (unsigned long) 0x8040201008040200);
}


TEST_F(bishop_generation, bishop_blockboard_5)
{
    auto result = board::magic::generate_attack_bishop(41, 0x805000508102040ULL);
    ASSERT_EQ(result, (unsigned long) 0x5000500000000);
}

TEST_F(bishop_generation, bishop_blockboard_6)
{
    auto result = board::magic::generate_attack_bishop(3, 0x1020400);
    ASSERT_EQ(result, (unsigned long) 0x8040201400);
}

TEST_F(bishop_generation, bishop_blockboard_7)
{
    auto result = board::magic::generate_attack_bishop(17, 0x20100805000008);
    ASSERT_EQ(result, (unsigned long) 0x5000508);
}



TEST_F(bishop_generation, bishop_blockboard_8)
{
    auto result = board::magic::generate_attack_bishop(61, 0x40000400000000);
    ASSERT_EQ(result, (unsigned long) 0x50080400000000);
}



TEST_F(bishop_generation, bishop_blockboard_9)
{
    auto result = board::magic::generate_attack_bishop(49, 0x500000000000000);
    ASSERT_EQ(result, (unsigned long) 0x500050810204080);
}



TEST_F(bishop_generation, bishop_blockboard_10)
{
    auto result = board::magic::generate_attack_bishop(26, 0x40001000000a0000);
    ASSERT_EQ(result, (unsigned long) 0x110a000a0000);
}
