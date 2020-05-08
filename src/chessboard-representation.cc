#include "chessboard-representation.hh"
#include "piece-type.hh"

#include <cstdlib>
#include <vector>
#include <utility>
#include <optional>
#include <iostream>
#include "color.hh"

namespace board
{
    Chessboard_rpr::Chessboard_rpr()
    {
        white_king = 1UL << 60;
        white_queen = 1UL << 59;
        white_knight = 1UL << 57 | 1UL << 62;
        white_pawn = 1UL << 48 | 1UL << 49 | 1UL << 50 | 1UL << 51 |
            1UL << 52 | 1UL << 53 | 1UL << 54 | 1UL << 55;
        white_bishop = 1UL << 58 | 1UL << 61;
        white_rook = 1UL << 56 | 1UL << 63;

        black_king = 1UL << 4;
        black_queen = 1UL << 3;
        black_knight = 1UL << 1 | 1UL << 6;
        black_pawn = 1UL << 8 | 1UL << 9 | 1UL << 10 | 1UL << 11 |
            1UL << 12 | 1UL << 13 | 1UL << 14 | 1UL << 15;
        black_bishop = 1UL << 2 | 1UL << 5;
        black_rook = 1UL << 0 | 1UL << 7;
    }

    void Chessboard_rpr::print()
    {
        std::string output;

        for (int i = 63; i >= 0; i--)
        {
            if ((white_king >> i) & 1U)
                output += "K ";
            else if ((white_queen >> i) & 1U)
                output += "Q ";
            else if ((white_bishop >> i) & 1U)
                output += "B ";
            else if ((white_rook >> i) & 1U)
                output += "R ";
            else if ((white_pawn >> i) & 1U)
                output += "P ";
            else if ((white_knight >> i) & 1U)
                output += "C ";
            else if ((black_king >> i) & 1U)
                output += "k ";
            else if ((black_queen >> i) & 1U)
                output += "q ";
            else if ((black_bishop >> i) & 1U)
                output += "b ";
            else if ((black_rook >> i) & 1U)
                output += "r ";
            else if ((black_pawn >> i) & 1U)
                output += "p ";
            else if ((black_knight >> i) & 1U)
                output += "c ";
            else
                output += "  ";

            if (i % 8 == 0)
                output += "\n";
        }
        std::cout << output;
    }

    void Chessboard_rpr::alter_rpr(Move move)
    {
        auto dest = move.dest_pos_get();
        auto src = move.start_pos_get();

        auto piece = at(src);
        auto capture = at(dest);

        unsigned int source_int = ((int) src.file_get() + 1) *
            (abs((int) src.rank_get() - 8)) - 1;
        unsigned int source_int = ((int) src.file_get() + 1) *
            (abs((int) src.rank_get() - 8)) - 1;

        if (move.piece_get() == PieceType::BISHOP)
    }

    void Chessboard_rpr::update_piece(PieceType type, Color color,
            int src, int dst)
    {
        if (type == PieceType::KING)
        {
            if (color == Color::BLACK)
        }
    }

    std::optional<std::pair<PieceType, Color>> Chessboard_rpr::at(Position pos)
    {
        unsigned int pos_int = ((int) pos.file_get() + 1) *
            (abs((int) pos.rank_get() - 8)) - 1;

        if ((white_king >> pos_int) & 1U)
            return std::pair<PieceType, Color>(PieceType::KING, Color::WHITE);
        else if ((white_queen >> pos_int) & 1U)
            return std::pair<PieceType, Color>(PieceType::QUEEN, Color::WHITE);
        else if ((white_bishop >> pos_int) & 1U)
            return std::pair<PieceType, Color>(PieceType::BISHOP, Color::WHITE);
        else if ((white_rook >> pos_int) & 1U)
            return std::pair<PieceType, Color>(PieceType::ROOK, Color::WHITE);
        else if ((white_pawn >> pos_int) & 1U)
            return std::pair<PieceType, Color>(PieceType::PAWN, Color::WHITE);
        else if ((white_knight >> pos_int) & 1U)
            return std::pair<PieceType, Color>(PieceType::KNIGHT, Color::WHITE);
        else if ((black_king >> pos_int) & 1U)
            return std::pair<PieceType, Color>(PieceType::KING, Color::BLACK);
        else if ((black_queen >> pos_int) & 1U)
            return std::pair<PieceType, Color>(PieceType::QUEEN, Color::BLACK);
        else if ((black_bishop >> pos_int) & 1U)
            return std::pair<PieceType, Color>(PieceType::BISHOP, Color::BLACK);
        else if ((black_rook >> pos_int) & 1U)
            return std::pair<PieceType, Color>(PieceType::ROOK, Color::BLACK);
        else if ((black_pawn >> pos_int) & 1U)
            return std::pair<PieceType, Color>(PieceType::PAWN, Color::BLACK);
        else if ((black_knight >> pos_int) & 1U)
            return std::pair<PieceType, Color>(PieceType::KNIGHT, Color::BLACK);
        else
            return std::nullopt;
    }
}