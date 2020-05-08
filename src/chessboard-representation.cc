#include "chessboard-representation.hh"
#include "piece-type.hh"

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
        std::vector<std::optional<std::pair<PieceType, Color>>> board(64);

        std::string output;

        for (int i = 63; i >= 0; i--)
        {
            if ((white_king >> i) & 1U)
            {
                auto tuple = std::pair(PieceType::KING, Color::WHITE);
                board.at(i) = tuple;
                output += "K";
            }
            else if ((white_queen >> i) & 1U)
            {
                auto tuple = std::pair(PieceType::QUEEN, Color::WHITE);
                board.at(i) = tuple;
                output += "Q";
            }
            else if ((white_bishop >> i) & 1U)
            {
                auto tuple = std::pair(PieceType::BISHOP, Color::WHITE);
                board.at(i) = tuple;
                output += "B";
            }
            else if ((white_rook >> i) & 1U)
            {
                auto tuple = std::pair(PieceType::ROOK, Color::WHITE);
                board.at(i) = tuple;
                output += "R";
            }
            else if ((white_pawn >> i) & 1U)
            {
                auto tuple = std::pair(PieceType::ROOK, Color::WHITE);
                board.at(i) = tuple;
                output += "P";
            }
            else if ((white_knight >> i) & 1U)
            {
                auto tuple = std::pair(PieceType::ROOK, Color::WHITE);
                board.at(i) = tuple;
                output += "C";
            }
            else if ((black_king >> i) & 1U)
            {
                auto tuple = std::pair(PieceType::KING, Color::BLACK);
                board.at(i) = tuple;
                output += "k";
            }
            else if ((black_queen >> i) & 1U)
            {
                auto tuple = std::pair(PieceType::QUEEN, Color::BLACK);
                board.at(i) = tuple;
                output += "q";
            }
            else if ((black_bishop >> i) & 1U)
            {
                auto tuple = std::pair(PieceType::BISHOP, Color::BLACK);
                board.at(i) = tuple;
                output += "b";
            }
            else if ((black_rook >> i) & 1U)
            {
                auto tuple = std::pair(PieceType::ROOK, Color::BLACK);
                board.at(i) = tuple;
                output += "r";
            }
            else if ((black_pawn >> i) & 1U)
            {
                auto tuple = std::pair(PieceType::ROOK, Color::BLACK);
                board.at(i) = tuple;
                output += "p";
            }
            else if ((black_knight >> i) & 1U)
            {
                auto tuple = std::pair(PieceType::ROOK, Color::BLACK);
                board.at(i) = tuple;
                output += "c";
            }
            else
            {
                board.at(i) = std::nullopt;
                output += " ";
            }

            if (i % 8 == 0)
            {
                output += "\n";
            }
        }
        std::cout << output;
    }
}