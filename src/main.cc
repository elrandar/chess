#include "listener-manager.hh"
#include <iostream>
#include "perft.hh"
#include "chess_engine/board/masks.hh"
#include "chess_engine/board/magic.hh"
#include "chess_engine/board/zobrist.hh"
#include "parsing/option.hh"
#include <chrono>
#include "parsing/perft-parser/perft-object.hh"
#include "utility/utype.hh"
#include "chess_engine/ai/search.hh"
#include "chess_engine/ai/uci.hh"
#include "chess_engine/ai/ai.hh"

int main(int argc, char **argv)
{
    board::Masks::init();
    board::BitboardOperations::init_ms1bTable();
    board::magic::build_table();
    board::Zobrist::initRandomArray();

    using namespace board;
    Option option;
    option.parse_options(argc, argv);

    if (option.isHelp())
    {
        Option::show_help();
    }
    else if (!option.getPgnPath().empty())
    {
        auto chessboard = board::Chessboard();
        auto listenerManager =
                listener::ListenerManager(option.getListenersVector(),
                                       chessboard);
        listenerManager.run_pgn(option.getPgnPath());
    }
    else if (!option.getPerftPath().empty())
    {
        int result = Perft::run_perft(option.getPerftPath());
        std::cout << result << '\n';
    }
    else if (option.timeTest)
    {
        auto start = std::chrono::high_resolution_clock::now();

        auto fenObj = perft_parser::parse_fen("r2qkb1r/p3nppp/2b1p3/3pP1B1/2p3P1/2N1QN2/PPP2P1P/3RR1K1 w kq - 2 15");
        auto cb = Chessboard(fenObj);
        std::cout << ai::search::findNextMove(cb).toString() << '\n';

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "\nIt took : " << duration.count() << " ms\n";
    }
    else
    {
        auto ewan = ai::Ai();
        ewan.run();
    }
    return 0;
}
