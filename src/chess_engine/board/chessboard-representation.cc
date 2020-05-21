#include "chessboard-representation.hh"
#include "piece-type.hh"

#include <cstdlib>
#include <vector>
#include <utility>
#include <optional>
#include <iostream>
#include <sstream>
#include "color.hh"

namespace board
{
    Chessboard_rpr::Chessboard_rpr()
    {
        boards = std::array<BitBoard, 12>();

        BitBoard white_queen = 1UL << 3u;
        boards[0] = white_queen;
        BitBoard white_rook = 1UL << 0u | 1UL << 7u;
        boards[1] = white_rook;
        BitBoard white_bishop = 1UL << 2u | 1UL << 5u;
        boards[2] = white_bishop;
        BitBoard white_knight = 1UL << 1u | 1UL << 6u;
        boards[3] = white_knight;
        BitBoard white_pawn = 1UL << 8u | 1UL << 9u | 1UL << 10u | 1UL << 11u |
                     1UL << 12u | 1UL << 13u | 1UL << 14u | 1UL << 15u;
        boards[4] = white_pawn;
        BitBoard white_king = 1UL << 4u;
        boards[5] = white_king;

        BitBoard black_queen = (1UL << 59u);
        boards[6] = black_queen;
        BitBoard black_rook = 1UL << 56u | 1UL << 63u;
        boards[7] = black_rook;
        BitBoard black_bishop = 1UL << 58u | 1UL << 61u;
        boards[8] = black_bishop;
        BitBoard black_knight = 1UL << 57u | 1UL << 62u;
        boards[9] = black_knight;
        BitBoard black_pawn = 1UL << 48u | 1UL << 49u | 1UL << 50u | 1UL << 51u |
                              1UL << 52u | 1UL << 53u | 1UL << 54u | 1UL << 55u;
        boards[10] = black_pawn;
        BitBoard black_king = 1UL << 60u;
        boards[11] = black_king;

        occupied = computeOccupied();
        BlackPieces = computeBlackPieces();
        WhitePieces = computeWhitePieces();
    }


    void Chessboard_rpr::print()
    {
        std::string line;
        std::vector<std::string> out;
        std::vector<char> chars {'Q', 'R', 'B', 'C', 'P', 'K', 'q', 'r', 'b', 'c', 'p', 'k'};

        for (size_t i = 0; i < 64; i++)
        {
            auto size = line.size();
            for (int j = 0; j < 12; ++j) {
                if ((boards.at(j) >> i) & 1U) {
                    line += chars.at(j);
                    line += " ";
                }
            }
            if (size == line.size())
                line += ". ";
            if ((i + 1) % 8 == 0) {
                line += "\n";
                out.insert(out.begin(), line);
                line = "";
            }
        }
        for (const auto& i : out)
            std::cout << i;
        std::cout << "\n";
    }


    std::optional<std::pair<PieceType, Color>> Chessboard_rpr::at(Position pos) const {
        unsigned int pos_int = static_cast<int>(pos.file_get()) + static_cast<int>(pos.rank_get()) * 8;

        for (size_t i = 0; i < 12; i++)
        {
            if ((boards.at(i) >> pos_int) & 1U)
            {
                auto type = static_cast<PieceType>(i % 6);
                auto color = (i >= 6) ? Color::BLACK : Color::WHITE;
                return std::pair<PieceType, Color>(type, color);
            }
        }
        return std::nullopt;
    }

    BitBoard Chessboard_rpr::computeOccupied() {
        BitBoard res = 0UL;
        for (auto bitboard : boards)
        {
            res |= bitboard;
        }
        return res;
    }

    BitBoard Chessboard_rpr::computeWhitePieces()
    {
        BitBoard res = 0UL;
        for (int i = 0; i < 6; i++)
        {
            res |= boards[i];
        }
        return res;
    }

    BitBoard Chessboard_rpr::computeBlackPieces()
    {
        BitBoard res = 0UL;
        for (int i = 6; i < 12; i++)
        {
            res |= boards[i];
        }
        return res;
    }

    std::string Chessboard_rpr::bitBoardPrint(BitBoard bitBoard)
    {
        std::vector<std::string> out;
        std::string line;
        for (unsigned i = 0; i < 64; i++)
        {
            if ((bitBoard >> i) & 1UL)
                line += "1 ";
            else
                line += ". ";

            if ((i + 1) % 8 == 0) {
                line += "\n";
                out.insert(out.begin(), line);
                line = "";
            }
        }

        std::stringstream stream;

        for (const auto& l : out)
        {
            stream << l;
        }
        stream << '\n';
        std::cout << stream.str();
        return stream.str();
    }

    BitBoard Chessboard_rpr::get(PieceType pieceType, Color color) {
        return boards[static_cast<int>(pieceType) + static_cast<int>(color) * 6];
    }

    Chessboard_rpr::Chessboard_rpr(perft_parser::FenObject fenObject)
    {
        boards = std::array<BitBoard, 12>();
        for (int i = 0; i < 12; i++)
            boards.at(i) = 0ul;

        for (uint8_t i = 0; i < 64; i++)
        {
            auto pieceSquare = fenObject[Position(i)];
            if (pieceSquare.has_value())
            {
                int boardNumber = static_cast<int>(pieceSquare.value().first)
                        + (pieceSquare.value().second == Color::WHITE ? 0 : 6);
                boards.at(boardNumber) |= 1ul << i;
            }
        }
        occupied = computeOccupied();
        BlackPieces = computeBlackPieces();
        WhitePieces = computeWhitePieces();
    }

    void Chessboard_rpr::update() {
        occupied = computeOccupied();
        WhitePieces = computeWhitePieces();
        BlackPieces = computeBlackPieces();
    }
}