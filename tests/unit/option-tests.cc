
#include <gtest/gtest.h>
#include "../../src/parsing/option.hh"


TEST(options, empty)
{
    auto options = Option();
    char* args[1];
    args[0] = (char*)("chessengine");
    options.parse_options(1, args);
    EXPECT_TRUE(options.getListenersVector().empty());
    EXPECT_EQ(options.getPerftPath(), "");
    EXPECT_EQ(options.getPgnPath(), "");
}
