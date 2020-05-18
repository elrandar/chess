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

    auto moves = chessboard.generate_legal_moves();
    chessboard.getBoardRpr().print();
    return 0;
}
