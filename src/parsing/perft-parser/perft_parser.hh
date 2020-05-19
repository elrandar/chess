#pragma once

#include <string>
#include <vector>
#include <err.h>
#include <cstring>
#include <iostream>
#include <optional>

#include "/../../chess_engine/board/color.hh"
#include "/../../chess_engine/board/position.hh"
#include "../../chess_engine/board/piece-type.hh"
#include "../../chess_engine/board/color.hh"

class FenRank
{
    public:
    FenRank(std::vector<board::PieceType, board::Color> pieces)
            : pieces_(pieces)
    {}

    FenRank();

    std::pair<board::PieceType, board::Color> operator[](board::File file);


private:
    std::vector<std::optional<std::pair<board::PieceType, board::Color>>> pieces_;
};

class FenObject : FenRank
{
    public:
    FenObject();
    FenObject(std::vector<FenRank> rank, board::Color side_to_move,
              std::vector<char> castling, std::optional<board::Position> en_passant_target)
            : rank_(rank)
            , side_to_move_(side_to_move)
            , castling_(castling)
            , en_passant_target_(en_passant_target)
    {}

    std::optional<std::pair<board::PieceType, board::Color>> operator[](board::Position position);
    board::Color side_to_move_get();
    std::vector<char> castling_get();
    std::optional<board::Position> en_passant_target_get();

private:
    std::vector<FenRank> rank_;
    board::Color side_to_move_;
    std::vector<char> castling_;
    std::optional<board::Position> en_passant_target_;
};

class PerftObject : FenObject
{
    PerftObject(FenObject fen, int depth)
            : fen_(fen)
            , depth_(depth)
    {}

    FenObject fen_get();
    int depth_get();

private:
    FenObject fen_;
    int depth_;
};

PerftObject parse_perft(std::string input);
FenObject parse_fen(std::vector<std::string> splited_input);