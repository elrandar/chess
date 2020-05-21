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
    };
}
