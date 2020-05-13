
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
        auto moves = std::vector<Move>();
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                auto val = generate_piece_moves(static_cast<PieceType>(j), static_cast<Color>(i));
                moves.insert(moves.end(), val.begin(), val.end());
            }
        }
        boardRpr.print();
        return moves;
    }

    std::vector<Move> Chessboard::generate_piece_moves(PieceType pieceType, Color color)
    {
        if (pieceType == PieceType::KNIGHT || pieceType == PieceType::KING)
        {
            return generate_knight_king_moves(pieceType, color);
        }
        return std::vector<Move>();
    }

    std::vector<Move> Chessboard::generate_knight_king_moves(PieceType pieceType, Color color)
    {
        auto moves = std::vector<Move>();
        // Get the BitBoard containing the kings
        BitBoard remainingPieces = boardRpr.get(pieceType, color);
        BitBoard eligibleSquares = color == Color::WHITE ? ~boardRpr.WhitePieces() : ~boardRpr.BlackPieces();

        // While there still are kings that have their moves to be generated
        while (remainingPieces != 0)
        {
            // Get the position of the least significant bit that is set
            unsigned pieceCell = ffsll(remainingPieces);

            BitBoard mask = pieceType == PieceType::KING ? Masks::king_attacks(pieceCell - 1)
                                                         : Masks::knight_attacks(pieceCell - 1);
            BitBoard generatedMoves = mask & eligibleSquares;

            if (generatedMoves != 0)
            {
                Chessboard_rpr::bitBoardPrint(generatedMoves);
            }
            while (generatedMoves != 0)
            {
                unsigned moveCell = ffsll(generatedMoves);
                moves.emplace_back(Position(pieceCell - 1), Position(moveCell - 1),
                                    pieceType);
                generatedMoves &= ~(1UL << (moveCell - 1));
            }
            // Unset the bit
            remainingPieces &= ~(1UL << (pieceCell - 1));
        }
        return moves;
    }

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