#include "rule.hh"
#include "masks.hh"
#include "magic.hh"

namespace board
{

    void
    bitboard_to_moves(unsigned initialPosition, BitBoard pushMovesBitboard, BitBoard attackMovesBitboard,
                      PieceType pieceType, Color color, std::vector<Move> &moves, Chessboard_rpr &boardRpr,
                      bool isDoublePush)
    {
        while (pushMovesBitboard != 0)
        {
            unsigned moveCell = BitboardOperations::bitScanForward(pushMovesBitboard);

            auto pos2 = Position(moveCell);

            auto move = Move(Position(initialPosition), Position(moveCell), pieceType);
            if (isDoublePush)
                move.setDoublePawnPush(true);
            // Is there promotion ?
            if (((color == Color::BLACK && pos2.rank_get() == Rank::ONE)
                || (color == Color::WHITE && pos2.rank_get() == Rank::EIGHT))
                && pieceType == PieceType::PAWN)
            {
                for (int i = 0; i < 4; i++)
                {
                    move.setPromotion(static_cast<PieceType>(i));
                    moves.push_back(move);
                }
            } else
                moves.push_back(move);
            pushMovesBitboard &= ~(1UL << (moveCell));
        }
        while (attackMovesBitboard != 0)
        {
            unsigned moveCell = BitboardOperations::bitScanForward(attackMovesBitboard);
            auto pos1 = Position(initialPosition);
            auto pos2 = Position(moveCell);
            auto capture = boardRpr.at(moveCell);
            if (isDoublePush)
            {
                capture = boardRpr.at(color == Color::WHITE ? moveCell - 8 : moveCell + 8);
            }
            auto move = Move(pos1, pos2, pieceType, capture.value().first);
            if (isDoublePush)
                move.setEnPassant(true);
            if (((color == Color::BLACK && pos2.rank_get() == Rank::ONE)
                 || (color == Color::WHITE && pos2.rank_get() == Rank::EIGHT))
                && pieceType == PieceType::PAWN)
            {
                for (int i = 0; i < 4; i++)
                {
                    move.setPromotion(static_cast<PieceType>(i));
                    moves.push_back(move);
                }
            } else
                moves.push_back(move);

            attackMovesBitboard &= ~(1UL << (moveCell));
        }
    }

    //==============================PAWN===========================================

    std::vector<Move> Rule::generate_pawn_moves_color(Chessboard_rpr &boardRpr, Color color, std::vector<Move> &moves,
                                                        BitBoard en_passant) {
        using bo = BitboardOperations;

        BitBoard eligibleSquares = ~boardRpr.occupied;
        BitBoard enemyPieces = color == Color::WHITE ? boardRpr.BlackPieces : boardRpr.WhitePieces;
        BitBoard pawns = boardRpr.get(PieceType::PAWN, color);


        while (pawns != 0)
        {
            unsigned pieceCell = bo::bitScanForward(pawns);

            BitBoard pushMoves;
            BitBoard doublePushMoves;
            BitBoard attackMoves;
            if (color == Color::WHITE)
            {
                // Single push
                pushMoves = bo::nortOne(1UL << pieceCell) & eligibleSquares;
                // Double push
                doublePushMoves = (bo::nortOne(pushMoves) & bo::rank4 & eligibleSquares);
            }
            else
            {
                // Single push
                pushMoves = bo::soutOne(1UL << pieceCell) & eligibleSquares;
                // Double push
                doublePushMoves = (bo::soutOne(pushMoves) & eligibleSquares & bo::rank5);
            }

            attackMoves = Masks::pawn_attacks(color, pieceCell) & enemyPieces;
            auto enPassantRank = color == Color::WHITE ? BitboardOperations::rank6 : BitboardOperations::rank3;
            auto enPassantattackMoves = Masks::pawn_attacks(color, pieceCell) & en_passant & enPassantRank;

            bitboard_to_moves(pieceCell, pushMoves, attackMoves, PieceType::PAWN, color, moves, boardRpr, false);
            bitboard_to_moves(pieceCell, doublePushMoves, enPassantattackMoves, PieceType::PAWN, color, moves, boardRpr, true);
            pawns &= ~(1UL << pieceCell);
        }
        return moves;
    }

    std::vector<Move> Rule::generate_pawn_moves(Chessboard &board)
    {
        Chessboard_rpr rpr = board.getBoardRpr();
        auto moves = std::vector<Move>();

        if (board.isWhiteTurn())
            generate_pawn_moves_color(rpr, Color::WHITE, moves, board.getEnPassant().top());
        else
            generate_pawn_moves_color(rpr, Color::BLACK, moves, board.getEnPassant().top());
        return moves;
    }

    //=============================== KNIGHT and KING ======================================================

    std::vector<Move> Rule::generate_knight_king_moves_color(PieceType pieceType, Color color, Chessboard_rpr &boardRpr,
                                                             std::vector<Move> &moves)
    {
        // Get the BitBoard containing the kings
        BitBoard remainingPieces = boardRpr.get(pieceType, color);
        BitBoard eligibleSquares = color == Color::WHITE ? ~boardRpr.WhitePieces : ~boardRpr.BlackPieces;
        BitBoard enemyPieces = color == Color::WHITE ? boardRpr.BlackPieces : boardRpr.WhitePieces;
        // While there still are kings that have their moves to be generated
        while (remainingPieces != 0)
        {
            // Get the position of the least significant bit that is set
            unsigned pieceCell = BitboardOperations::bitScanForward(remainingPieces);

            BitBoard mask = pieceType == PieceType::KING ? Masks::king_attacks(pieceCell)
                                                         : Masks::knight_attacks(pieceCell);
            BitBoard generatedMoves = mask & eligibleSquares;

            bitboard_to_moves(pieceCell, generatedMoves & ~enemyPieces,
                              generatedMoves & enemyPieces, pieceType, color, moves, boardRpr, false);
            // Unset the bit
            remainingPieces &= ~(1UL << (pieceCell));
        }
        return moves;
    }

    std::vector<Move> Rule::generate_king_moves(Chessboard &board) {
        auto boardRpr = board.getBoardRpr();
        auto moves = std::vector<Move>();
        if (board. isWhiteTurn())
            generate_knight_king_moves_color(PieceType::KING, Color::WHITE, boardRpr, moves);
        else
            generate_knight_king_moves_color(PieceType::KING, Color::BLACK, boardRpr, moves);
        return moves;
    }

    std::vector<Move> Rule::generate_knight_moves(Chessboard &board) {
        auto boardRpr = board.getBoardRpr();
        auto moves = std::vector<Move>();
        if (board.isWhiteTurn())
            generate_knight_king_moves_color(PieceType::KNIGHT, Color::WHITE, boardRpr, moves);
        else
            generate_knight_king_moves_color(PieceType::KNIGHT, Color::BLACK, boardRpr, moves);
        return moves;
    }

    std::vector<Move> Rule::generate_bishop_moves(Chessboard &board) {
        auto boardRpr = board.getBoardRpr();
        auto moves = std::vector<Move>();
        if (board.isWhiteTurn())
            generate_bishop_rook_moves_color(boardRpr, Color::WHITE, PieceType::BISHOP, moves, false);
        else
            generate_bishop_rook_moves_color(boardRpr, Color::BLACK, PieceType::BISHOP, moves, false);
        return moves;
    }

    std::vector<Move>
    Rule::generate_bishop_rook_moves_color(Chessboard_rpr &chessboardRpr, Color color, PieceType pieceType,
                                           std::vector<Move> &moves, bool isQueen)
    {
        BitBoard remainingPieces = isQueen ? chessboardRpr.get(PieceType::QUEEN, color) :
                                            chessboardRpr.get(pieceType, color);
        BitBoard eligibleSquares = color == Color::WHITE ? ~chessboardRpr.WhitePieces : ~chessboardRpr.BlackPieces;
        BitBoard enemyPieces = color == Color::WHITE ? chessboardRpr.BlackPieces : chessboardRpr.WhitePieces;
        // While there still are pieces that have their moves to be generated
        while (remainingPieces != 0)
        {
            // Get the position of the least significant bit that is set
            unsigned pieceCell = BitboardOperations::bitScanForward(remainingPieces);

            BitBoard mask;
            BitBoard magic;
            unsigned shift;

            if (pieceType == PieceType::BISHOP)
            {
                mask = Masks::bishop_attacks(pieceCell);
                magic = magic::BMagics[pieceCell];
                shift = magic::BShift[pieceCell];
            } else
            {
                mask = Masks::rook_attacks(pieceCell);
                magic = magic::RMagics[pieceCell];
                shift = magic::RShift[pieceCell];
            }

            auto index = ((chessboardRpr.occupied & mask) * magic) >> shift;

            BitBoard generatedMoves;
            if (pieceType == PieceType::BISHOP)
                generatedMoves = magic::BishopAttacksSquare[pieceCell][index] & eligibleSquares;
            else
                generatedMoves = magic::RookAttacksSquare[pieceCell][index] & eligibleSquares;

            bitboard_to_moves(pieceCell, generatedMoves & ~enemyPieces,
                              generatedMoves & enemyPieces, isQueen ? PieceType::QUEEN : pieceType,
                              color, moves, chessboardRpr, false);
            // Unset the bit
            remainingPieces &= ~(1UL << (pieceCell));
        }
        return moves;
    }

    std::vector<Move> Rule::generate_rook_moves(Chessboard &board) {
        auto boardRpr = board.getBoardRpr();
        auto moves = std::vector<Move>();
        if (board.isWhiteTurn())
            generate_bishop_rook_moves_color(boardRpr, Color::WHITE, PieceType::ROOK, moves, false);
        else
            generate_bishop_rook_moves_color(boardRpr, Color::BLACK, PieceType::ROOK, moves, false);
        return moves;
    }

    std::vector<Move> Rule::generate_queen_moves(Chessboard &board) {
        auto boardRpr = board.getBoardRpr();
        auto moves = std::vector<Move>();
        if (board.isWhiteTurn())
        {
            generate_bishop_rook_moves_color(boardRpr, Color::WHITE, PieceType::ROOK, moves, true);
            generate_bishop_rook_moves_color(boardRpr, Color::WHITE, PieceType::BISHOP, moves, true);
        } else
        {
            generate_bishop_rook_moves_color(boardRpr, Color::BLACK, PieceType::ROOK, moves, true);
            generate_bishop_rook_moves_color(boardRpr, Color::BLACK, PieceType::BISHOP, moves, true);
        }
        return moves;
    }

}


