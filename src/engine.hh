#pragma once

#include "listener/Handle_listeners.hh"
#include "chess_engine/board/chessboard.hh"
#include <string>
#include <fstream>
#include <utility>
#include <vector>

class Engine
{

private:

    std::string pgn_path;
    listener::HandleListener handler;

public:

  Engine(std::string pgn, listener::HandleListener& handler)
    : pgn_path(std::move(pgn))
    , handler(handler)
    {}

  bool start_game(board::Chessboard chessboard, listener::HandleListener handle);
  bool do_castling(board::Chessboard chessboard, listener::HandleListener handle, board::Move move,board::Rank rank);
  void disqualify(listener::HandleListener handle, board::Color color);

  static std::vector<board::Move> get_moves(const std::string& file);
};