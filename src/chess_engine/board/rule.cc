#include "rule.hh"
#include "masks.hh"

namespace board
{

    void
    bitboard_to_moves(unsigned initialPosition, BitBoard pushMovesBitboard, BitBoard attackMovesBitboard,
                                  PieceType pieceType, std::vector<Move> &moves, Chessboard_rpr& boardRpr)
    {
        while (pushMovesBitboard != 0)
        {
            unsigned moveCell = BitboardOperations::bitScanForward(pushMovesBitboard);
            moves.emplace_back(Position(initialPosition), Position(moveCell),
                               pieceType);
            pushMovesBitboard &= ~(1UL << (moveCell));
        }
        while (attackMovesBitboard != 0)
        {
            unsigned moveCell = BitboardOperations::bitScanForward(attackMovesBitboard);
            auto pos1 = Position(initialPosition);
            auto pos2 = Position(moveCell);
            auto capture = boardRpr.at(moveCell);
            moves.emplace_back(pos1, pos2, pieceType, capture.value().first);
            attackMovesBitboard &= ~(1UL << (moveCell));
        }
    }

    //==============================PAWN===========================================

    std::vector<Move> Rule::generate_pawn_moves_color(Chessboard_rpr &boardRpr, Color color, std::vector<Move> &moves) {
        using bo = BitboardOperations;

        BitBoard eligibleSquares = ~boardRpr.occupied();
        BitBoard enemyPieces = color == Color::WHITE ? boardRpr.BlackPieces() : boardRpr.WhitePieces();
        BitBoard pawns = boardRpr.get(PieceType::PAWN, color);


        while (pawns != 0)
        {
            unsigned pieceCell = bo::bitScanForward(pawns);

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

            bitboard_to_moves(pieceCell, pushMoves, attackMoves, PieceType::PAWN, moves, boardRpr);
            pawns &= ~(1UL << pieceCell);
        }
        return moves;
    }

    std::vector<Move> Rule::generate_pawn_moves(Chessboard &board)
    {
        Chessboard_rpr rpr = board.getBoardRpr();
        auto moves = std::vector<Move>();
        generate_pawn_moves_color(rpr, Color::WHITE, moves);
        generate_pawn_moves_color(rpr, Color::BLACK, moves);
        return moves;
    }

    //=============================== KNIGHT and KING ======================================================

    std::vector<Move> Rule::generate_knight_king_moves_color(PieceType pieceType, Color color, Chessboard_rpr &boardRpr,
                                                             std::vector<Move> &moves)
    {
        // Get the BitBoard containing the kings
        BitBoard remainingPieces = boardRpr.get(pieceType, color);
        BitBoard eligibleSquares = color == Color::WHITE ? ~boardRpr.WhitePieces() : ~boardRpr.BlackPieces();
        BitBoard enemyPieces = color == Color::WHITE ? boardRpr.BlackPieces() : boardRpr.WhitePieces();
        // While there still are kings that have their moves to be generated
        while (remainingPieces != 0)
        {
            // Get the position of the least significant bit that is set
            unsigned pieceCell = BitboardOperations::bitScanForward(remainingPieces);

            BitBoard mask = pieceType == PieceType::KING ? Masks::king_attacks(pieceCell)
                                                         : Masks::knight_attacks(pieceCell);
            BitBoard generatedMoves = mask & eligibleSquares;

            bitboard_to_moves(pieceCell, generatedMoves & ~enemyPieces,
                    generatedMoves & enemyPieces, pieceType, moves, boardRpr);
            // Unset the bit
            remainingPieces &= ~(1UL << (pieceCell));
        }
        return moves;
    }

    std::vector<Move> Rule::generate_king_moves(Chessboard &board) {
        auto boardRpr = board.getBoardRpr();
        auto moves = std::vector<Move>();
        generate_knight_king_moves_color(PieceType::KING, Color::WHITE, boardRpr, moves);
        generate_knight_king_moves_color(PieceType::KING, Color::BLACK, boardRpr, moves);
        return moves;
    }

    std::vector<Move> Rule::generate_knight_moves(Chessboard &board) {
        auto boardRpr = board.getBoardRpr();
        auto moves = std::vector<Move>();
        generate_knight_king_moves_color(PieceType::KNIGHT, Color::WHITE, boardRpr, moves);
        generate_knight_king_moves_color(PieceType::KNIGHT, Color::BLACK, boardRpr, moves);
        return moves;
    }

}


