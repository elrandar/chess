#include "listener-manager.hh"
#include <iostream>
#include "chess_engine/board/masks.hh"
#include "chess_engine/board/magic.hh"
#include "parsing/option.hh"
#include "parsing/perft-parser/perft-object.hh"
#include "utility/utype.hh"

int main(int argc, char **argv)
{
    board::Masks::init();
    board::BitboardOperations::init_ms1bTable();
    board::magic::build_table();

    using namespace board;
    Option option;
    option.parse_options(argc, argv);


    auto chessboard = board::Chessboard();
    if (option.isHelp())
    {
        option.show_help();
    }
    else if (!option.getPgnPath().empty())
    {
        if (!option.getListenersVector().empty())
        {
            auto big_boi = listener::ListenerManager(option.getListenersVector(), chessboard);
            big_boi.run_pgn(option.getPgnPath());
        }
    }
    else if (!option.getPerftPath().empty())
    {
        // do perft stuff
    }
    else
    {
        // enter IA mode
    }
    return 0;
}
