#include "listener-manager.hh"
#include <iostream>
#include "chess_engine/board/masks.hh"
#include "chess_engine/board/magic.hh"
#include "parsing/option.hh"
#include "parsing/perft-parser/perft-object.hh"
#include "utility/utype.hh"

int main()
{
    board::Masks::init();
    board::BitboardOperations::init_ms1bTable();

    using namespace board;

    auto perft = perft_parser::parse_perft("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8 1");
    std::string out;
    std::vector<char> chars {'Q', 'R', 'B', 'N', 'P', 'K', 'q', 'r', 'b', 'n', 'p', 'k'};

    for (int i = 7; i >= 0; i--)
    {
        for (int j = 0; j < 8; j++)
        {
            auto piece = perft.fen_get()[Position(static_cast<File>(j), static_cast<Rank>(i))];
            if (piece.has_value())
                out += chars.at(static_cast<int>(piece->first) + (piece->second == Color::WHITE ? 0 : 6));
            else
                out += '.';
            out += ' ';
        }
        out += '\n';
    }
    std::cout << out << '\n';


//    Option option;
//    board::Chessboard chessboard;
//    if (argc == 1)
//    {
//        ai::init("ChessAi");
//        std::string my_move;
//        std::string board;
//        while(true)
//        {
//            board = get_board();
//            std::string get = get_input();
//            std::cin >> my_move;
//            play_move(my_move);
//        }
//
//    }
//    else
//    {
////      ListenerManager handler(option.getListenersVector());
////      ListenerManager engine(option.getPgnPath(), handler);
////      engine.run_pgn(chessboard, handler);
//
//        if (!option.parse_options(argc, argv))
//        {
//            std::cout << "Options are not valid" << "\n";
//        }
//        if (option.isHelp())
//        {
//            option.show_help();
//            return 0;
//        }
//        if (option.getPgnPath().empty())
//        {
//            std::cerr << "PGN file invalid" << "\n";
//        }
//    }

    return 0;
}
