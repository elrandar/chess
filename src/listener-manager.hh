#pragma once

#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "chess_engine/board/chessboard.hh"
#include "listener/listener.hh"
#include "parsing/pgn-parser/pgn-move.hh"

namespace listener
{
    class ListenerManager : public board::ChessboardInterface
    {
    private:
        board::Chessboard chessboard_;
        std::vector<Listener*> listeners;
        std::vector<void*> handlers;

        std::vector<uint64_t> positions;

    public:
        ListenerManager(const std::vector<std::string>& files,
                        board::Chessboard& chessboard);
        ~ListenerManager() override;

        // Operator that has to be implemented for the ChessboardInterface
        opt_piece_t operator[](const board::Position& position) const override;

        // Functions used to run
        bool run_pgn(const std::string& pgn_path);
        void disqualify(board::Color color);

        static board::Move pgnMoveToMove(board::PgnMove pgnMove,
                                         board::Chessboard chessboard);

        // Wrapper Functions to act on all the listeners
        // =============================================================

        void register_board(const board::ChessboardInterface& board_interface);
        void on_game_finished();
        void on_piece_moved(board::PieceType piece, const board::Position& from,
                            const board::Position& to);
        void on_piece_taken(board::PieceType piece, const board::Position& at);
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

        static int is_pgn_move_castling(board::PgnMove pgnMove);

        bool isThreefold() const;
    };
} // namespace listener
