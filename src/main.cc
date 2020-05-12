#include <iostream>
#include "chessboard-representation.hh"
#include "position.hh"
#include "move.hh"
#include "chessboard.hh"
#include "masks.hh"

int main()
{
    board::Masks::init();

    using namespace board;
    auto cb = Chessboard();


    cb.generate_legal_moves();
    return 0;
}
