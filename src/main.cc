#include <iostream>
#include "chessboard-representation.hh"
#include "position.hh"
#include "move.hh"



    int main(void) {

        using namespace board;
        Chessboard_rpr board = Chessboard_rpr();
        board.print();

        for (auto bit : board.boards)
        {
            Chessboard_rpr::bitBoardPrint(bit);
            std::cout << "\n";
        }

        Chessboard_rpr::bitBoardPrint(board.generalBoard());

        Position src = Position(File::A, Rank::TWO);
        Position dst = Position(File::A, Rank::SEVEN);


        Move move =  Move(src, dst, board::PieceType::PAWN);
        board.alter_rpr(move);
        for (auto bit : board.boards)
        {
            Chessboard_rpr::bitBoardPrint(bit);
            std::cout << "\n";
        }

        Chessboard_rpr::bitBoardPrint(board.generalBoard());
        board.print();
        return 0;
    }
