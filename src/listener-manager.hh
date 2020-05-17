#pragma once

#include "listener/listener.hh"
#include "chess_engine/board/chessboard.hh"
#include <string>
#include <fstream>
#include <utility>
#include <vector>

namespace listener
{
    class ListenerManager : public board::ChessboardInterface
    {

    private:

        board::Chessboard chessboard_;
        std::vector<Listener*> listeners;
        std::vector<void*> handlers;

    public:
        ListenerManager(const std::vector<std::string>& files, board::Chessboard chessboard);
        ~ListenerManager() override;

        // Operator that has to be implemented for the ChessboardInterface
        opt_piece_t operator[](const board::Position &position) const override;

        // Functions used to run
        bool run_pgn(std::string pgn_path);
        bool do_castling(board::Move move, board::Rank rank);
        void disqualify(board::Color color);


        static std::vector<board::Move> pgn_to_moves(const std::string& file);
        bool check_on_position(std::vector<board::Move> legal_moves, board::Position king_pos, board::Color color);
        bool is_king_oncheck(board::Color color);
        bool piece_can_move(std::vector<board::Move> legal_moves,board::PieceType piece, board::Color color);


        // Wrapper Functions to act on all the listeners =============================================================

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
    };
}
