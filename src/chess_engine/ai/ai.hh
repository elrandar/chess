#pragma once

#include "../board/chessboard.hh"

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
        static board::Color ai_color;
        static const int ai_depth = 5;
        static std::vector<std::pair<double, int>> pair_list;
    };
}
