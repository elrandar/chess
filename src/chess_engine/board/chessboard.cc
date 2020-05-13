
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
        } else if (pieceType == PieceType::PAWN)
        {
            return generate_pawn_moves(color);
        }
        return std::vector<Move>();
    }

    void
    Chessboard::bitboard_to_moves(unsigned initialPosition, BitBoard pushMovesBitboard, BitBoard attackMovesBitboard,
                                  PieceType pieceType, std::vector<Move> &moves)
    {
        while (pushMovesBitboard != 0)
        {
            unsigned moveCell = ffsll(pushMovesBitboard) - 1;
            moves.emplace_back(Position(initialPosition), Position(moveCell),
                               pieceType);
            pushMovesBitboard &= ~(1UL << (moveCell));
        }
        while (attackMovesBitboard != 0)
        {
            unsigned moveCell = ffsll(attackMovesBitboard) - 1;
            auto pos1 = Position(initialPosition);
            auto pos2 = Position(moveCell);
            auto capture = boardRpr.at(moveCell);
            moves.emplace_back(pos1, pos2, pieceType, capture.value().first);
            attackMovesBitboard &= ~(1UL << (moveCell));
        }
    }

    std::vector<Move> Chessboard::generate_pawn_moves(Color color) {
        using bo = BitboardOperations;

        BitBoard eligibleSquares = ~boardRpr.occupied();
        BitBoard enemyPieces = color == Color::WHITE ? boardRpr.BlackPieces() : boardRpr.WhitePieces();
        BitBoard pawns = boardRpr.get(PieceType::PAWN, color);

        auto moves = std::vector<Move>();

        while (pawns != 0)
        {
            unsigned pieceCell = ffsll(pawns) - 1;

            BitBoard pushMoves;
            BitBoard attackMoves;
            if (color == Color::WHITE)
            {
                // Single push
                pushMoves = bo::nortOne(1UL << pieceCell) & eligibleSquares;
                // Double push
                pushMoves |= bo::nortOne(pushMoves) & bo::rank4 & eligibleSquares;
            }
            else
            {
                // Single push
                pushMoves = bo::soutOne(1UL << pieceCell) & eligibleSquares;
                // Double push
                pushMoves |= bo::soutOne(pushMoves) & eligibleSquares & bo::rank5;
            }

            attackMoves = Masks::pawn_attacks(color, pieceCell) & enemyPieces;

            if (attackMoves != 0)
                Chessboard_rpr::bitBoardPrint(attackMoves);
//            Chessboard_rpr::bitBoardPrint(pushMoves);
            bitboard_to_moves(pieceCell, pushMoves, attackMoves, PieceType::PAWN, moves);
            pawns &= ~(1UL << pieceCell);
        }
        return moves;
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