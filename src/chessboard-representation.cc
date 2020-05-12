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
        boards = std::vector<BitBoard>(0);

        BitBoard white_queen = 1UL << 3u;
        boards.push_back(white_queen);
        BitBoard white_rook = 1UL << 0u | 1UL << 7u;
        boards.push_back(white_rook);
        BitBoard white_bishop = 1UL << 2u | 1UL << 5u;
        boards.push_back(white_bishop);
        BitBoard white_knight = 1UL << 1u | 1UL << 6u;
        boards.push_back(white_knight);
        BitBoard white_pawn = 1UL << 8u | 1UL << 9u | 1UL << 10u | 1UL << 11u |
                     1UL << 12u | 1UL << 13u | 1UL << 14u | 1UL << 15u;
        boards.push_back(white_pawn);
        BitBoard white_king = 1UL << 4u | 1UL << 22u;
        boards.push_back(white_king);

        BitBoard black_queen = 1UL << 59u;
        boards.push_back(black_queen);
        BitBoard black_rook = 1UL << 56u | 1UL << 63u;
        boards.push_back(black_rook);
        BitBoard black_bishop = 1UL << 58u | 1UL << 61u;
        boards.push_back(black_bishop);
        BitBoard black_knight = 1UL << 57u | 1UL << 62u;
        boards.push_back(black_knight);
        BitBoard black_pawn = 1UL << 48u | 1UL << 49u | 1UL << 50u | 1UL << 51u |
                              1UL << 52u | 1UL << 53u | 1UL << 54u | 1UL << 55u;
        boards.push_back(black_pawn);
        BitBoard black_king = 1UL << 60u;
        boards.push_back(black_king);
    }

    Chessboard_rpr::Chessboard_rpr(Move move)
        : Chessboard_rpr()
    {
        execute_move(move);
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
    }

    void Chessboard_rpr::execute_move(Move move)
    {
        auto dest = move.dest_pos_get();
        auto src = move.start_pos_get();

        auto piece = this->at(src);
        auto capture = this->at(dest);

        unsigned int source_int = static_cast<int>(src.file_get()) + static_cast<int>(src.rank_get()) * 8;
        unsigned int dest_int = static_cast<int>(dest.file_get()) + static_cast<int>(dest.rank_get()) * 8;

        int board_index = static_cast<int>(piece->first) + (piece->second == Color::WHITE ? 0 : 6);

        boards.at(board_index) = (boards.at(board_index) & ~(1UL << source_int)) | 1UL << dest_int;
        if (capture.has_value())
        {
            int capture_board_index = static_cast<int>(capture->first) + (capture->second == Color::WHITE ? 0 : 6);
            boards.at(capture_board_index) &= ~(1UL << dest_int);
        }
    }

    std::optional<std::pair<PieceType, Color>> Chessboard_rpr::at(Position pos)
    {
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

    BitBoard Chessboard_rpr::occupied() {
        BitBoard res = 0UL;
        for (auto bitboard : boards)
        {
            res |= bitboard;
        }
        return res;
    }

    BitBoard Chessboard_rpr::WhitePieces()
    {
        BitBoard res = 0UL;
        for (int i = 0; i < 6; i++)
        {
            res |= boards[i];
        }
        return res;
    }

    BitBoard Chessboard_rpr::BlackPieces()
    {
        BitBoard res = 0UL;
        for (int i = 6; i < 12; i++)
        {
            res |= boards[i];
        }
        return res;
    }

    void Chessboard_rpr::bitBoardPrint(BitBoard bitBoard)
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
        for (const auto& l : out)
        {
            std::cout << l;
        }
        std::cout << '\n';
    }

    BitBoard Chessboard_rpr::get(PieceType pieceType, Color color) {
        return boards[static_cast<int>(pieceType) + static_cast<int>(color) * 8];
    }
}