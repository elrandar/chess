#pragma once

#include <utility>

#include "fen-rank.hh"

namespace perft_parser
{
    class FenObject
    {
    private:
        std::vector<FenRank> ranks_;
        board::Color side_to_move_;
        std::vector<char> castling_;
        std::optional<board::Position> en_passant_target_;

    public:
        explicit FenObject(std::string fenString);
        std::optional<std::pair<board::PieceType, board::Color>>
        operator[](board::Position position);

        board::Color side_to_move_get();
        std::vector<char> castling_get();
        std::optional<board::Position> en_passant_target_get();

        void print();
    };
} // namespace perft_parser
