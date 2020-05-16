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
    board::magic::build_table();

    using namespace board;
    auto cb = Chessboard();


    auto moves = cb.generate_legal_moves();
    cb.getBoardRpr().print();

//    test blockboard generation
//    test move generation
    auto result = board::magic::generate_attack_bishop(27, 0x8041020000102041);
    Chessboard_rpr::bitBoardPrint(0x8041020000102041);
    Chessboard_rpr::bitBoardPrint(result);

    auto index = ((0x8041020000102041 & 0x7e7e7e7e7e7e00UL) * board::magic::BMagics[27]) >> board::magic::BShift[27];
    Chessboard_rpr::bitBoardPrint(board::magic::BishopAttacksSquare[27][index]);


    return 0;
}
