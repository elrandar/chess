#include "chessboard-representation.hh"
#include "position.hh"
#include "move.hh"



    int main(void) {

        using namespace board;
        Chessboard_rpr board = Chessboard_rpr();
        board.print();

        Position src = Position(File::A, Rank::TWO);
        Position dst = Position(File::A, Rank::SEVEN);


        Move move =  Move(src, dst, board::PieceType::PAWN);
        board.alter_rpr(move);
        board.print();
        return 0;
    }
