#pragma once

#include <vector>
#include <memory>
#include "../board/move.hh"

namespace ai
{
    class Node {

    public:

        explicit Node(float value) : value_(value) {
            children = std::vector<std::shared_ptr<Node>>();
        }

        Node() {
            value_ = 0;
        }

        float value_;
        std::vector<std::shared_ptr<Node>> children;
    };

    class Gtree {
    public:

        std::shared_ptr<Node> node_;
        std::vector<board::Move> moves_;

    };
}
