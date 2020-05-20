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

    auto perft = perft_parser::parse_perft("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 1");
    perft.fen_get().print();


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
