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
#include "chess_engine/ai/uci.hh"

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
        ai::init("E-won McGregor");
        board::Chessboard cb = board::Chessboard();
        while (!cb.is_checkmate())
        {
            std::string board = ai::get_board();
            auto boardFen = perft_parser::parse_fen(board);
            cb = board::Chessboard(boardFen);
            auto move = ai::search::findNextMove(cb);
            ai::play_move(move.toString());
        }
    }
    return 0;
}
