#include "search.hh"
#include "evaluation.hh"
#include <limits>
#include <utility>
namespace ai
{
    std::shared_ptr<Node> search::build_node(board::Chessboard chessboard, int depth, Color myColor,
                                             const std::string& moveThatGotMeHere)
    {
        auto currentColor = chessboard.isWhiteTurn() ? Color::WHITE : Color::BLACK;
        auto moveList = chessboard.generate_legal_moves();

        auto tree = std::make_shared<Node>(moveThatGotMeHere, currentColor);

        auto checkmated = chessboard.is_checkmate(moveList);

        if (depth == 0 || checkmated)
        {
            auto evaluation = Evaluation(chessboard);
            if (checkmated && currentColor == board::Color::WHITE)
                evaluation.whiteIsCheckmated = true;
            else if (checkmated && currentColor == board::Color::BLACK)
                evaluation.blackIsCheckmated = true;
            tree->value_ = evaluation.rate_chessboard(myColor);
            return tree;
        }

        for (auto move : moveList)
        {
            chessboard.do_move(move);
            tree->children.push_back(build_node(chessboard, depth - 1, myColor,
                                moveThatGotMeHere + '/' + move.toString()));

            chessboard.undo_move(move);
        }
        return tree;
    }

    Gtree search::build_tree(board::Chessboard chessboard, int depth, Color myColor)
    {
        std::shared_ptr<Node> node = build_node(chessboard, depth, myColor, "start");
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
            float value = - std::numeric_limits<float>::infinity();
            size_t value_index = 0;
            size_t i = 0;
            for (const auto& child : tree->children) {
                auto oldValue = value;
                value = std::max(value, minMax(child, false));
                if (oldValue != value)
                    value_index = i;
                i++;
            }
            tree->value_ = value;
            tree->move_ = tree->children[value_index]->move_;
            return value;
        }
        else
        {
            float value = std::numeric_limits<float>::infinity();
            size_t value_index = 0;
            size_t i = 0;
            for (const auto& child : tree->children) {
                auto oldValue = value;
                value = std::min(value, minMax(child, true));
                if (oldValue != value)
                    value_index = i;
                i++;
            }
            tree->value_ = value;
            tree->move_ = tree->children[value_index]->move_;
            return value;
        }
    }

    Move search::findNextMove(board::Chessboard chessboard, int depth)
    {
        auto colorToMaximize = chessboard.isWhiteTurn() ? Color::WHITE : Color::BLACK;
        auto tree = build_tree(std::move(chessboard), depth, colorToMaximize);
        float best_move_val = minMax(tree.node_, true);

        Node bestNode = Node(best_move_val, "                                                                                                                                                                                                                  ",
                board::Color::WHITE);
        Move bestNodeMove = Move(0,0);

        int i = 0;
        for (const auto& child : tree.node_->children)
        {
            if (child->value_ == best_move_val && child->move_.size() < bestNode.move_.size())
            {
                bestNode = *child;
                bestNodeMove = tree.moves_[i];
            }
            i++;
        }
        return bestNodeMove;
    }
}