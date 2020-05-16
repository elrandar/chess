#pragma once
#include <vector>
#include <string>
#include "listener.hh"
#include "../chess_engine/board/piece-type.hh"
#include "../chess_engine/board/position.hh"
#include "../chess_engine/board/color.hh"
#include "../chess_engine/board/chessboard-interface.hh"

namespace listener
{
    class HandleListener
    {
    public:

        HandleListener(std::vector<std::string> paths);
        ~HandleListener();

        void register_board(const board::ChessboardInterface& board_interface);
        void on_game_finished();
        void on_piece_moved(board::PieceType piece,
                            const board::Position& from,
                            const board::Position& to);
        void on_piece_taken(board::PieceType piece,
                            const board::Position& at);
        void on_piece_promoted(board::PieceType piece,
                               const board::Position& at);
        void on_kingside_castling(board::Color color);
        void on_queenside_castling(board::Color color);
        void on_player_check(board::Color color);
        void on_player_mat(board::Color color);
        void on_player_pat(board::Color color);
        void on_player_timeout(board::Color color);
        void on_player_disqualified(board::Color color);
        void on_draw();

    private:

        std::vector<Listener*> listeners;
        std::vector<void*> handlers;

        void register_board(const ChessboardInterface &board_interface);
    };
}
