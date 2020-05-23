
#include "fen-object.hh"

#include <algorithm>
#include <iostream>

namespace perft_parser
{
    std::optional<std::pair<board::PieceType, board::Color>>
    FenObject::operator[](board::Position position)
    {
        auto rankIndex = static_cast<int>(position.rank_get());
        return ranks_.at(rankIndex)[position.file_get()];
    }

    board::Color FenObject::side_to_move_get()
    {
        return side_to_move_;
    }

    std::vector<char> FenObject::castling_get()
    {
        return castling_;
    }

    std::optional<board::Position> FenObject::en_passant_target_get()
    {
        return en_passant_target_;
    }

    FenObject::FenObject(std::string fenString)
    {
        // parse ranks
        size_t cursor = 0;
        size_t nextSlashPos;
        while ((nextSlashPos = fenString.find('/', cursor))
               != std::string::npos)
        {
            ranks_.emplace_back(
                fenString.substr(cursor, nextSlashPos - cursor));
            cursor = nextSlashPos + 1;
        }
        size_t nextSpacePos = fenString.find(' ', cursor);
        ranks_.emplace_back(fenString.substr(cursor, nextSpacePos - cursor));
        // reverse the ranks
        std::reverse(ranks_.begin(), ranks_.end());

        // side to move
        cursor = nextSpacePos + 1;
        side_to_move_ = fenString[cursor] == 'w' ? board::Color::WHITE
                                                 : board::Color::BLACK;

        // castling ability
        cursor += 2;
        nextSpacePos = fenString.find(' ', cursor);
        while (cursor != nextSpacePos)
        {
            if (fenString.at(cursor)
                != '-') // Check that castling ability is not empty
                castling_.push_back(fenString.at(cursor));
            cursor++;
        }

        // en passant target
        cursor++;
        if (fenString.at(cursor) != '-')
        {
            auto enPassantTargetFile =
                static_cast<board::File>(fenString.at(cursor) - 'a');
            cursor++;
            auto enPassantTargetRank = fenString.at(cursor) == '3'
                ? board::Rank::THREE
                : board::Rank::SIX;
            en_passant_target_ =
                board::Position(enPassantTargetFile, enPassantTargetRank);
        } else
            en_passant_target_ = std::nullopt;
    }

    void FenObject::print()
    {
        std::string out;
        std::vector<char> chars{'Q', 'R', 'B', 'N', 'P', 'K',
                                'q', 'r', 'b', 'n', 'p', 'k'};

        for (int i = 7; i >= 0; i--)
        {
            for (int j = 0; j < 8; j++)
            {
                auto piece = (*this)[board::Position(
                    static_cast<board::File>(j), static_cast<board::Rank>(i))];
                if (piece.has_value())
                    out += chars.at(
                        static_cast<int>(piece->first)
                        + (piece->second == board::Color::WHITE ? 0 : 6));
                else
                    out += '.';
                out += ' ';
            }
            out += '\n';
        }
        std::cout << out << '\n';
    }
} // namespace perft_parser
