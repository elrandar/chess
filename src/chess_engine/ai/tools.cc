//
// Created by mathieut on 5/24/20.
//

#include "tools.hh"

#include "../board/color.hh"
#include "../board/magic.hh"
#include "../board/masks.hh"
#include "../board/piece-type.hh"
namespace ai
{
    int tools::nb_piece(board::PieceType pieceType, board::Color color,
                        board::Chessboard_rpr& rpr)
    {
        int isBlack = color == board::Color::BLACK ? 6 : 0;
        return __builtin_popcount(
            rpr.boards[static_cast<int>(pieceType) + isBlack]);
    }

    int tools::gamePhase(board::Chessboard_rpr& rpr)
    {
        int nb_queens =
            nb_piece(board::PieceType::QUEEN, board::Color::WHITE, rpr)
            + nb_piece(board::PieceType::QUEEN, board::Color::BLACK, rpr);
        int nb_rooks =
            nb_piece(board::PieceType::ROOK, board::Color::WHITE, rpr)
            + nb_piece(board::PieceType::ROOK, board::Color::BLACK, rpr);
        int nb_bishops =
            nb_piece(board::PieceType::BISHOP, board::Color::WHITE, rpr)
            + nb_piece(board::PieceType::BISHOP, board::Color::BLACK, rpr);
        int nb_knights =
            nb_piece(board::PieceType::KNIGHT, board::Color::WHITE, rpr)
            + nb_piece(board::PieceType::KNIGHT, board::Color::BLACK, rpr);
        return 24 - (4 * nb_queens) - (2 * nb_rooks) - nb_bishops - nb_knights;
    }
    int tools::fpap(board::Color color, board::Chessboard_rpr& rpr,
                    board::BitBoard fileToCheck)
    {
        auto pawnBoard = rpr.get(board::PieceType::PAWN, color);
        auto pawnOnFileToCheck = fileToCheck & pawnBoard;
        auto indexPawnOnFileToCheck =
            board::BitboardOperations::bitScanForward(pawnOnFileToCheck);
        if (indexPawnOnFileToCheck == -1)
            return -36;
        auto pawnRank = indexPawnOnFileToCheck / 8;
        auto distanceTo = color == board::Color::WHITE ? 7 : 0;
        auto distance = std::abs(distanceTo - pawnRank);
        return -36 + (distance * distance);
    }

    int tools::hpap(board::Color color, board::Chessboard_rpr& rpr,
                    board::BitBoard fileToCheck)
    {
        auto eval = 0;
        auto opponent_color = color == board::Color::WHITE
            ? board::Color::BLACK
            : board::Color::WHITE;
        auto opponentPawnsBoard =
            rpr.get(board::PieceType::PAWN, opponent_color);
        auto opponentPawnsFile = opponentPawnsBoard & fileToCheck;
        auto rank4OpPawn = board::BitboardOperations::rank4 & opponentPawnsFile;
        auto rank5OpPawn = board::BitboardOperations::rank5 & opponentPawnsFile;
        auto rank6OpPawn = board::BitboardOperations::rank6 & opponentPawnsFile;
        if (rank4OpPawn)
            eval += -10;
        if (rank5OpPawn)
            eval += -30;
        if (rank6OpPawn)
            eval += -60;
        return eval;
    }
} // namespace ai