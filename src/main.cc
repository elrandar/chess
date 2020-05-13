#include <iostream>
#include "chess_engine/board/chessboard-representation.hh"
#include "chess_engine/board/position.hh"
#include "chess_engine/board/move.hh"
#include "chess_engine/board/chessboard.hh"
#include "chess_engine/board/masks.hh"

int main()
{
    board::Masks::init();

    using namespace board;
    auto cb = Chessboard();


    auto moves = cb.generate_legal_moves();

    return 0;
}
