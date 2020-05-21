#include "listener-manager.hh"
#include <iostream>
#include "perft.hh"
#include "chess_engine/board/masks.hh"
#include "chess_engine/board/magic.hh"
#include "parsing/option.hh"
#include <chrono>
#include "parsing/perft-parser/perft-object.hh"
#include "utility/utype.hh"
#include "chess_engine/ai/search.hh"

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
        int result = Perft::run_perft(option.getPerftPath());
        std::cout << result << '\n';
    }
    else
    {
        board::Chessboard cb = board::Chessboard(perft_parser::parse_fen("4kb1r/1Np2R2/r2P2n1/1p2nP2/p1P1p1p1/1Q1KP3/P3P1PP/2B2BNR w - - 0 1"));
        auto start = std::chrono::high_resolution_clock::now();

        ai::search::findNextMove(cb).print();

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << " It took : " << duration.count() << " ms\n";
    }
    return 0;
}
