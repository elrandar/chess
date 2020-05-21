#include "search.hh"
#include "evaluation.hh"
#include <limits>
#include <utility>
namespace ai
{
    std::shared_ptr<Node> search::build_node(board::Chessboard chessboard, int depth)
    {
        Color side = chessboard.isWhiteTurn() ? Color::WHITE : Color::BLACK;

        auto moveList = chessboard.generate_legal_moves();

        auto tree = std::make_shared<Node>();

        if (depth == 0)
        {
            auto evaluation = Evaluation(chessboard);
            tree->value_ = evaluation.rate_chessboard(side);
            return tree;
        }

        for (auto move : moveList)
        {
            chessboard.do_move(move);
            tree->children.push_back(build_node(chessboard, depth - 1));
            chessboard.undo_move(move);
        }
        return tree;
    }

    Gtree search::build_tree(board::Chessboard chessboard, int depth)
    {
        std::shared_ptr<Node> node = build_node(chessboard, depth);
        auto moves = chessboard.generate_legal_moves();
        Gtree tree = Gtree();
        tree.node_ = std::move(node);
        tree.moves_ = std::move(moves);
        return tree;
    }

    float search::minMax(const std::shared_ptr<Node>& tree, bool maximizing)
    {
        if (tree->children.empty())
            return tree->value_;
        if (maximizing)
        {
            float value = - std::numeric_limits<float>::max();
            for (const auto& child : tree->children)
                value = std::max(value, minMax(child, false));
            tree->value_ = value;
            return value;
        }
        else
        {
            float value = std::numeric_limits<float>::max();
            for (const auto& child : tree->children)
                value = std::min(value, minMax(child, true));
            tree->value_ = value;
            return value;
        }
    }

    Move search::findNextMove(board::Chessboard chessboard)
    {
        auto tree = build_tree(std::move(chessboard), 3);
        float best_move_val = minMax(tree.node_, true);
        int i = 0;
        for (const auto& child : tree.node_->children)
        {
            if (child->value_ == best_move_val)
                return tree.moves_[i];
            i++;
        }
        return Move(1, 2, board::PieceType::ROOK);
    }
}