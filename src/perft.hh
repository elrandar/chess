#pragma once

#include <string>
#include "chess_engine/board/chessboard.hh"

class Perft {

private:
    static int perft(board::Chessboard chessboard, int depth);
public:
    static int run_perft(std::string filePath);

};


