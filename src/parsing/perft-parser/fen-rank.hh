#pragma once

#include <optional>
#include <utility>
#include <vector>

#include "../../chess_engine/board/color.hh"
#include "../../chess_engine/board/piece-type.hh"
#include "../../chess_engine/board/position.hh"

namespace perft_parser
{
    class FenRank
    {
    private:
        std::vector<std::optional<std::pair<board::PieceType, board::Color>>>
            pieces_;

    public:
        explicit FenRank(const std::string& rankString);

        std::optional<std::pair<board::PieceType, board::Color>>
        operator[](board::File file);
    };
} // namespace perft_parser
