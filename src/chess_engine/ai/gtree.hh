#pragma once

#include <utility>
#include <vector>
#include <memory>
#include "../board/move.hh"
#include "../board/color.hh"

namespace ai
{
    class Node {

    public:

        explicit Node(float value, std::string move, board::Color color) : value_(value), move_(std::move(move)) {
            color_ = color;
            children = std::vector<std::shared_ptr<Node>>();
        }

        Node(std::string move, board::Color color) : move_(std::move(move)) {
            value_ = 0;
            color_ = color;
        }

        float value_;
        std::string move_;
        board::Color color_;

        std::vector<std::shared_ptr<Node>> children;
    };

    class Gtree {
    public:

        std::shared_ptr<Node> node_;
        std::vector<board::Move> moves_;

    };
}
