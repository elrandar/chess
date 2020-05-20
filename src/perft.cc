
#include "perft.hh"
#include "parsing/perft-parser/perft-object.hh"
#include "chess_engine/board/chessboard.hh"
#include <istream>
#include <fstream>
#include <sstream>

int Perft::run_perft(std::string filePath) {
    // parse and get chessboard
    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string perftString = buffer.str();
    auto perftObj = perft_parser::parse_perft(perftString);

    auto chessboard = board::Chessboard(perftObj.fen_get());



    return perft(chessboard, perftObj.depth_get());
}

int Perft::perft(board::Chessboard chessboard, int depth)
{
    std::vector<board::Move> moveList;
    unsigned nodes = 0;

    if (depth == 0) return 1;

    moveList = chessboard.generate_legal_moves();
    for (auto move : moveList)
    {
        chessboard.do_move(move);
        nodes += perft(chessboard, depth - 1);
        chessboard.undo_move(move);
    }
    return nodes;
}
