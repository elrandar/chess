#pragma once

#include "../board/chessboard.hh"
#include "../board/zobrist.hh"

namespace ai
{
    class Ai
    {
    private:
        board::Chessboard chessboard;
        bool gameFinished;

    public:
        Ai();

        void run();

        void update_board(const std::string& boardString);

        void print_board();

        static std::vector<uint64_t> boardPositionsHash;
        static board::Color ai_color;
        static int ai_depth;
        static std::vector<std::pair<double, int>> pair_list;

        static unsigned long last_hash();
    };
} // namespace ai
