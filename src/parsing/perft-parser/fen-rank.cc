
#include <algorithm>
#include "fen-rank.hh"

namespace perft_parser
{
    std::optional<std::pair<board::PieceType, board::Color>> FenRank::operator[] (board::File file)
    {
        return pieces_.at(static_cast<int>(file));
    }

    FenRank::FenRank(const std::string& rankString) {
        for (char c : rankString)
        {
            if (c >= '1' && c <= '8')
            {
                for (int i = 0; i < (c - '0'); i++)
                    pieces_.emplace_back();
            }
            else
            {
                auto pieceType = board::char_to_piece(toupper(c));
                auto color = c > 'Z' ? board::Color::BLACK : board::Color::WHITE;
                pieces_.emplace_back(std::pair(pieceType, color));
            }
        }
    }
}

