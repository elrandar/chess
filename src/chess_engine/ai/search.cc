#include "search.hh"
#include "evaluation.hh"
#include "ai.hh"
#include <limits>
#include <utility>
#include <iostream>

namespace ai
{
    std::shared_ptr<Node> search::build_node(board::Chessboard chessboard, int depth,
                                             const std::string& moveThatGotMeHere)
    {
        auto currentColor = chessboard.isWhiteTurn() ? Color::WHITE : Color::BLACK;
        auto moveList = chessboard.generate_legal_moves();

        auto tree = std::make_shared<Node>(moveThatGotMeHere, currentColor);

        auto checkmated = chessboard.is_checkmate(moveList);
        auto pat = chessboard.is_pat(moveList);

        if (depth == 0 || checkmated || pat)
        {
            tree->checkmated_ = checkmated;
            tree->chessboard_ = chessboard;
            return tree;
        }

        for (auto move : moveList)
        {
            chessboard.do_move(move);
            tree->children.push_back(build_node(chessboard, depth - 1,moveThatGotMeHere + '/' + move.toString()));

            chessboard.undo_move(move);
        }
        return tree;
    }

    Gtree search::build_tree(board::Chessboard chessboard, int depth)
    {
        std::shared_ptr<Node> node = build_node(chessboard, depth, "start");
        auto moves = chessboard.generate_legal_moves();
        Gtree tree = Gtree();
        tree.node_ = std::move(node);
        tree.moves_ = std::move(moves);
        return tree;
    }

    double search::minMax(const std::shared_ptr<Node> &tree, bool maximizing, double alpha, double beta)
    {
        if (tree->children.empty())
        {
            if (!tree->chessboard_.has_value())
            {
                std::cout << tree->move_ << '\n';
            }
            auto evaluation = Evaluation(tree->chessboard_.value());
            if (tree->checkmated_ && tree->color_ == board::Color::WHITE)
                evaluation.whiteIsCheckmated = true;
            else if (tree->checkmated_ && tree->color_ == board::Color::BLACK)
                evaluation.blackIsCheckmated = true;
            tree->value_ = evaluation.rate_chessboard(Ai::ai_color);
            return tree->value_;
        }
        if (maximizing)
        {
            double value = - std::numeric_limits<double>::infinity();
            size_t value_index = 0;
            size_t i = 0;
            for (const auto& child : tree->children) {
                auto oldValue = value;
                value = std::max(value, minMax(child, false, alpha, beta));
                alpha = std::max(alpha, value);
                if (oldValue != value)
                    value_index = i;
                i++;
                if (alpha >= beta)
                    break;
            }
            tree->value_ = value;
            tree->move_ = tree->children[value_index]->move_;
            return value;
        }
        else
        {
            double value = std::numeric_limits<double>::infinity();
            size_t value_index = 0;
            size_t i = 0;
            for (const auto& child : tree->children) {
                auto oldValue = value;
                value = std::min(value, minMax(child, true, alpha, beta));
                beta = std::min(beta, value);
                if (oldValue != value)
                    value_index = i;
                i++;
                if (alpha >= beta)
                    break
;
            }
            tree->value_ = value;
            tree->move_ = tree->children[value_index]->move_;
            return value;
        }
    }

    Move search::findNextMove(board::Chessboard chessboard, int depth)
    {
        Ai::ai_color = chessboard.isWhiteTurn() ? board::Color::WHITE : board::Color::BLACK;
        auto tree = build_tree(std::move(chessboard), depth);
        auto inf = std::numeric_limits<double>::infinity();
        double best_move_val = minMax(tree.node_, true, -inf, inf);

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