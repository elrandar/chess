#pragma once

#include "Handle_listeners.hh"
#include "chessboard.hh"
#include "pgn-parser.hh"
#include "move.hh"
#include <string>
#include <fstream>
#include <utility>
#include <vector>

class Engine
{

public:

  Engine(std::string pgn, HandleListener& handler)
    : pgn_path(std::move(pgn))
    , handler(handler)
    {}

  bool start_game(board::Chessboard chessboard, HandleListener handler);

  static std::vector<board::Move> get_moves(const std::string& file);


private:

  std::string pgn_path;
  HandleListener handler;

};