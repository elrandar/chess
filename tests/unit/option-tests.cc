
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

TEST(options, help_short)
{
    auto options = Option();
    auto args = std::vector<char*>();
    args.push_back((char*)"chessengine");
    args.push_back((char*)"-h");
    options.parse_options(args.size(), args.data());
    EXPECT_TRUE(options.getListenersVector().empty());
    EXPECT_EQ(options.getPerftPath(), "");
    EXPECT_EQ(options.getPgnPath(), "");
    EXPECT_TRUE(options.isHelp());
}

TEST(options, help_long)
{
    auto options = Option();
    auto args = std::vector<char*>();
    args.push_back((char*)"chessengine");
    args.push_back((char*)"--help");
    options.parse_options(args.size(), args.data());
    EXPECT_TRUE(options.getListenersVector().empty());
    EXPECT_EQ(options.getPerftPath(), "");
    EXPECT_EQ(options.getPgnPath(), "");
    EXPECT_TRUE(options.isHelp());
}

TEST(options, pgn)
{
    auto options = Option();
    auto args = std::vector<char*>();
    args.push_back((char*)"chessengine");
    args.push_back((char*)"--pgn");
    args.push_back((char*)"lol.pgn");
    options.parse_options(args.size(), args.data());
    EXPECT_TRUE(options.getListenersVector().empty());
    EXPECT_EQ(options.getPerftPath(), "");
    EXPECT_EQ(options.getPgnPath(), args[2]);
    EXPECT_FALSE(options.isHelp());
}

TEST(options, pgn_with_listeners)
{
    auto options = Option();
    auto args = std::vector<char*>();
    args.push_back((char*)"chessengine");
    args.push_back((char*)"--pgn");
    args.push_back((char*)"lol.pgn");
    args.push_back((char*)"-l");
    args.push_back((char*)"./libbasic-output.so");
    options.parse_options(args.size(), args.data());
    EXPECT_TRUE(options.getListenersVector().size() == 1);
    EXPECT_EQ(options.getPerftPath(), "");
    EXPECT_EQ(options.getPgnPath(), args[2]);
    EXPECT_FALSE(options.isHelp());
}

TEST(options, pgn_with_listeners_long)
{
    auto options = Option();
    auto args = std::vector<char*>();
    args.push_back((char*)"chessengine");
    args.push_back((char*)"--pgn");
    args.push_back((char*)"lol.pgn");
    args.push_back((char*)"--listeners");
    args.push_back((char*)"./libbasic-output.so");
    args.push_back((char*)"./libmdr-output.so");
    options.parse_options(args.size(), args.data());
    EXPECT_TRUE(options.getListenersVector().size() == 2);
    EXPECT_EQ(options.getPerftPath(), "");
    EXPECT_EQ(options.getPgnPath(), args[2]);
    EXPECT_FALSE(options.isHelp());
}

TEST(options, perft)
{
    auto options = Option();
    auto args = std::vector<char*>();
    args.push_back((char*)"./chessengine");
    args.push_back((char*)"--perft");
    args.push_back((char*)"./lol.perft");
    options.parse_options(args.size(), args.data());
    EXPECT_TRUE(options.getListenersVector().empty());
    EXPECT_EQ(options.getPerftPath(), args[2]);
    EXPECT_EQ(options.getPgnPath(), "");
    EXPECT_FALSE(options.isHelp());
}
