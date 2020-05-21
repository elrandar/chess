
#include <list>
#include <iostream>
#include "ai.hh"
#include "uci.hh"
#include "../../parsing/perft-parser/perft-object.hh"
#include "search.hh"
#include "../board/move.hh"

void ai::Ai::run() {

    ai::init("E-won McGregor");
    while (!gameFinished)
    {
        auto board = get_board();

        update_board(board);

        board::Move move = ai::search::findNextMove(chessboard);

        ai::play_move(move.toString());
    }
}

ai::Ai::Ai() {
    chessboard = board::Chessboard();
    gameFinished = false;
}

void ai::Ai::update_board(const std::string& boardString) {
    auto firstSpacePos = boardString.find(' ');
    auto nextSpacePos = boardString.find(' ', firstSpacePos + 1);

    std::string positionType =
            boardString.substr(firstSpacePos + 1, nextSpacePos - firstSpacePos - 1);
    if (positionType == "startpos")
        chessboard = board::Chessboard();
    else if (positionType == "fen")
    {
        std::string fenStr;
        size_t firstMovePos;
        if ((firstMovePos = boardString.find("moves")) != std::string::npos)
            fenStr = boardString.substr(nextSpacePos + 5, firstMovePos - 1);
        else
            fenStr = boardString.substr(nextSpacePos + 5);
        auto fenObj = perft_parser::parse_fen(fenStr);
        chessboard = board::Chessboard(fenObj);
    }

    auto vec = std::vector<board::Move>();

    size_t cursor = 0;
    while ((cursor = boardString.find("moves", cursor)) != std::string::npos)
    {
        cursor += 6;
        auto srcFile = static_cast<board::File>(boardString.at(cursor) - 'a');
        cursor++;
        auto srcRank = static_cast<board::Rank>(boardString.at(cursor) - '0' - 1);
        cursor++;
        auto dstFile = static_cast<board::File>(boardString.at(cursor) - 'a');
        cursor++;
        auto dstRank = static_cast<board::Rank>(boardString.at(cursor) - '0' - 1);
        auto move = board::Move(board::Position(srcFile, srcRank),
                                board::Position(dstFile, dstRank));
        vec.push_back(move);
    }
    std::cout << "mais ptn\n";
}


