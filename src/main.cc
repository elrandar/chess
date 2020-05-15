#include <iostream>
#include <bitset>
#include "chess_engine/board/chessboard-representation.hh"
#include "chess_engine/board/position.hh"
#include "chess_engine/board/move.hh"
#include "chess_engine/board/chessboard.hh"
#include "chess_engine/board/masks.hh"
#include "chess_engine/board/magic.hh"

int main()
{
    board::Masks::init();
    board::BitboardOperations::init_ms1bTable();

    using namespace board;
    auto cb = Chessboard();


    auto moves = cb.generate_legal_moves();


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
