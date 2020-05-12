
#include <strings.h>
#include <iostream>
#include "masks.hh"
#include "chessboard.hh"

namespace board
{
    void Chessboard::do_move(Move move)
    {
        boardRpr.execute_move(move);
    }

    std::vector<Move> Chessboard::generate_legal_moves()
    {
        return generate_king_moves();
    }

    std::vector<Move> Chessboard::generate_king_moves()
    {
        auto moves = std::vector<Move>();
        // Get the BitBoard containing the kings
        BitBoard remainingKings = boardRpr.get(PieceType::KING, Color::WHITE);
        BitBoard eligibleSquares = ~boardRpr.WhitePieces();

        // While there still are kings that have their moves to be generated
        while (remainingKings != 0)
        {
            Chessboard_rpr::bitBoardPrint(remainingKings);
            // Get the position of the least significant bit that is set
            unsigned bitPos = ffsll(remainingKings);

            BitBoard generatedMoves = Masks::king_attacks(bitPos - 1) & eligibleSquares;
            Chessboard_rpr::bitBoardPrint(generatedMoves);

            while (generatedMoves != 0)
            {
                unsigned movePos = ffsll(generatedMoves);
                moves.emplace_back(Position(bitPos - 1), Position(movePos - 1), PieceType::KING);
                generatedMoves &= ~(1UL << (movePos - 1));
            }
            std::cout << bitPos << '\n';

            // Unset the bit
            remainingKings &= ~(1UL << (bitPos - 1));
            Chessboard_rpr::bitBoardPrint(remainingKings);
        }
        return std::vector<Move>();
    }

//    bool is_move_legal(Move move)
//    {
//
//    }

    Chessboard::Chessboard()
    {
        white_turn_ = true;
        white_king_castling_ = false;
        white_queen_castling_ = false;
        black_king_castling_ = false;
        black_queen_castling_ = false;
        turn_ = 1;
        last_fifty_turn_ = 50;
        boardRpr = Chessboard_rpr();
    }


}