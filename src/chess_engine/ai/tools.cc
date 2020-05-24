//
// Created by mathieut on 5/24/20.
//

#include "tools.hh"
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
} // namespace ai