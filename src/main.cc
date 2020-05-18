#include "listener-manager.hh"
#include <iostream>
#include "chess_engine/board/masks.hh"
#include "chess_engine/board/magic.hh"
#include "parsing/option.hh"

int main()
{
    board::Masks::init();
    board::BitboardOperations::init_ms1bTable();
    board::magic::build_table();

    using namespace board;

    Option option;
    auto chessboard = board::Chessboard();
//    ListenerManager handler(option.getListenersVector());
//    ListenerManager engine(option.getPgnPath(), handler);
//    engine.run_pgn(chessboard, handler);

    auto moves = chessboard.generate_legal_moves();
    chessboard.getBoardRpr().print();


    //test blockboard generation
//    std::bitset<64> bigbit = Masks::rook_attack[27];
//    auto nb_bbit = bigbit.count();
//    for (int i = 0; i < (1 << nb_bbit); i++)
//    {
//        Chessboard_rpr::bitBoardPrint(board::magic::generate_blockboard(i, Masks::rook_attack[27]));
//    }
    //test move generation
    Chessboard_rpr::bitBoardPrint(board::magic::generate_attack_rook(0, 0x100000020));
    return 0;
}
