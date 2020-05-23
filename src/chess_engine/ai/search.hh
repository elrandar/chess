
#include "gtree.hh"
#include "../board/chessboard.hh"
#include "../board/color.hh"
#include "../board/move.hh"

namespace ai::search
{
    std::shared_ptr<Node> build_node(board::Chessboard chessboard, int depth, const std::string& moveThatGotMeHere);
    Gtree build_tree(board::Chessboard chessboard, int depth);

    std::pair<double, int>
    minMax(bool maximizing, int depth, double alpha, double beta,
           board::Chessboard chessboard);

    board::Move findNextMove(board::Chessboard chessboard);
}