
#include "gtree.hh"
#include "../board/chessboard.hh"
namespace ai::search
{
    std::shared_ptr<Node> build_node(board::Chessboard chessboard, int depth);
    Gtree build_tree(board::Chessboard chessboard, int depth);

    float minMax(const std::shared_ptr<Node>& tree, bool maximizing);

    board::Move findNextMove(board::Chessboard chessboard);
}