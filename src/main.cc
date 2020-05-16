#include "engine.hh"
#include "chessboard.hh"
#include <iostream>
#include "chess_engine/board/masks.hh"
#include "chess_engine/board/magic.hh"
#include "parsing/option.hh"

int main(int argc, char** argv)
{
    board::Masks::init();
    board::BitboardOperations::init_ms1bTable();

    using namespace board;

    Option option;
    board::Chessboard chessboard;
    HandleListener handler(option.getListenersVector());
    Engine engine(option.getPgnPath(), handler);
    engine.start_game(chessboard, handler);

    if (!option.parse_options(argc, argv))
    {
        std::cout << "Options are not valid" << "\n";
    }
    if (option.isHelp())
    {
        option.show_help();
        return 0;
    }
    if (option.getPgnPath().empty())
    {
        std::cerr << "PGN file invalid" << "\n";
    }

    return 0;
}
