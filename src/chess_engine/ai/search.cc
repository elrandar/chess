#include "search.hh"
#include "evaluation.hh"
#include "ai.hh"
#include <limits>
#include <utility>
#include <iostream>

namespace ai
{
    std::shared_ptr<Node> search::build_node(board::Chessboard chessboard,
                            int depth,
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

    void print_tab(int depth, const std::vector<board::Move>& moves) {
        for (auto move : moves)
        {
            for (int i = 0; i < depth; i++)
            {
                std::cout << '\t';
            }
            std::cout << move.toString() << '\n';
        }
    }
    std::pair<double, int>
    search::minMax(bool maximizing, int depth, double alpha, double beta,
                   board::Chessboard chessboard)
    {
        // generate the legal moves
        auto moveList = chessboard.generate_legal_moves();
        auto currentColor = chessboard.isWhiteTurn() ? Color::WHITE : Color::BLACK;

        auto checkmated = chessboard.is_checkmate(moveList);
        auto pat = chessboard.is_pat(moveList);

        if (moveList.empty() || depth == Ai::ai_depth || checkmated || pat)
        {
            // evaluate node
            auto evaluation = Evaluation(chessboard);
            if (checkmated && currentColor == board::Color::WHITE)
                evaluation.whiteIsCheckmated = true;
            else if (checkmated && currentColor == board::Color::BLACK)
                evaluation.blackIsCheckmated = true;
            return std::pair<double, int>(evaluation.rate_chessboard(Ai::ai_color), depth);
        }
        if (maximizing)
        {
            double value = - std::numeric_limits<double>::infinity();
            int value_depth = 0;
            for (auto& move : moveList) {
                chessboard.do_move(move);
                auto rec = minMax(false, depth + 1, alpha,
                            beta, chessboard);
                if (depth == 0)
                    Ai::pair_list.push_back(rec);
                auto oldValue = value;
                value = std::max(value, rec.first);
                if (oldValue != value)
                    value_depth = rec.second;
                chessboard.undo_move(move);
                alpha = std::max(alpha, value);
                if (alpha >= beta)
                    break;
            }

            return std::make_pair(value, value_depth);
        }
        else
        {
            double value = std::numeric_limits<double>::infinity();
            int value_depth = 0;
            for (auto& move : moveList) {

                chessboard.do_move(move);

                auto rec = minMax(true, depth + 1, alpha, beta,
                        chessboard);
                if (depth == 0)
                    Ai::pair_list.push_back(rec);
                auto oldValue = value;
                value = std::min(value, rec.first);
                if (oldValue != value)
                    value_depth = rec.second;
                chessboard.undo_move(move);

                beta = std::min(beta, value);
                if (alpha >= beta)
                    break;
            }
            return std::make_pair(value, value_depth);
        }
    }

    Move search::findNextMove(board::Chessboard chessboard)
    {
        Ai::ai_color = chessboard.isWhiteTurn() ? board::Color::WHITE : board::Color::BLACK;

        // compute next moves
        auto moveList = chessboard.generate_legal_moves();

        // find optimal move value

        Ai::pair_list = std::vector<std::pair<double, int>>();

        auto inf = std::numeric_limits<double>::infinity();
        auto best_move_pair = minMax(true, 0, -inf, inf,
                                      std::move(chessboard));


        // find optimal move from optimal move value
        std::pair<double, int> bestPair = std::make_pair(best_move_pair.first, 400);
        auto bestMove = Move(0, 0);

        int i = 0;
        for (auto pair : Ai::pair_list)
        {
            if (pair.first == bestPair.first && pair.second < bestPair.second)
            {
                bestPair = pair;
                bestMove = moveList[i];
            }
            i++;
        }
        return bestMove;
    }

}