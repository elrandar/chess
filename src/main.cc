#include "listener-manager.hh"
#include <iostream>
#include "perft.hh"
#include "chess_engine/board/masks.hh"
#include "chess_engine/board/magic.hh"
#include "parsing/option.hh"
#include <chrono>
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

    if (option.isHelp())
    {
        option.show_help();
    }
    else if (!option.getPgnPath().empty())
    {
        auto chessboard = board::Chessboard();
        auto listenerManager = listener::ListenerManager(option.getListenersVector(), chessboard);
        listenerManager.run_pgn(option.getPgnPath());
    }
    else if (!option.getPerftPath().empty())
    {
//        auto start = std::chrono::high_resolution_clock::now();
        int result = Perft::run_perft(option.getPerftPath());
//        auto stop = std::chrono::high_resolution_clock::now();
//        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
//        std::cout << result << '\n';
//        std::cout << "It took : " << duration.count() << " ms\n";
    }
    else
    {
        // enter IA mode
    }
    return 0;
}
