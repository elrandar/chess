#pragma once
#include <vector>
#include <string>
#include "listener.hh"
#include "chessboard-interface.hh"
#include "piece-type.hh"

  class HandleListener : public listener::Listener
  {
  public:

    HandleListener(std::vector<std::string> paths);
    ~HandleListener();

    void register_board(const board::ChessboardInterface& board_interface);
    void on_game_finished();
    void on_piece_moved(const board::PieceType piece,
                        const board::Position& from,
                        const board::Position& to);
    void on_piece_taken(const board::PieceType piece,
                        const board::Position& at);
    void on_piece_promoted(const board::PieceType piece,
                           const board::Position& at);
    void on_kingside_castling(const board::Color color);
    void on_queenside_castling(const board::Color color);
    void on_player_check(const board::Color color);
    void on_player_mat(const board::Color color);
    void on_player_pat(const board::Color color);
    void on_player_timeout(const board::Color color);
    void on_player_disqualified(const board::Color color);
    void on_draw();

  private:

    std::vector<Listener*> listeners;
    std::vector<void*> handlers;
  };