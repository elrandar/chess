
#include "gtree.hh"
#include "../board/chessboard.hh"
#include "../board/color.hh"

namespace ai::search
{
    std::shared_ptr<Node> build_node(board::Chessboard chessboard, int depth, board::Color myColor);
    Gtree build_tree(board::Chessboard chessboard, int depth, board::Color myColor);

    float minMax(const std::shared_ptr<Node>& tree, bool maximizing);

    board::Move findNextMove(board::Chessboard chessboard);
}