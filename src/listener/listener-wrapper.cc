#include "../listener-manager.hh"

namespace listener
{
    void ListenerManager::disqualify(board::Color color)
    {
        on_player_disqualified(color);
        on_game_finished();
    }

    void ListenerManager::register_board(
        const board::ChessboardInterface& board_interface)
    {
        for (auto& listener : listeners)
            listener->register_board(board_interface);
    }

    void ListenerManager::on_game_finished()
    {
        for (auto& listener : listeners)
            listener->on_game_finished();
    }

    void ListenerManager::on_piece_moved(const board::PieceType piece,
                                         const board::Position& from,
                                         const board::Position& to)
    {
        for (auto& listener : listeners)
            listener->on_piece_moved(piece, from, to);
    }

    void ListenerManager::on_piece_taken(const board::PieceType piece,
                                         const board::Position& at)
    {
        for (auto& listener : listeners)
            listener->on_piece_taken(piece, at);
    }

    void ListenerManager::on_piece_promoted(const board::PieceType piece,
                                            const board::Position& at)
    {
        for (auto& listener : listeners)
        {
            listener->on_piece_promoted(piece, at);
        }
    }

    void ListenerManager::on_kingside_castling(const board::Color color)
    {
        for (auto& listener : listeners)
            listener->on_kingside_castling(color);
    }

    void ListenerManager::on_queenside_castling(const board::Color color)
    {
        for (auto& listener : listeners)
            listener->on_queenside_castling(color);
    }

    void ListenerManager::on_player_check(const board::Color color)
    {
        for (auto& listener : listeners)
            listener->on_player_check(color);
    }

    void ListenerManager::on_player_mat(const board::Color color)
    {
        for (auto& listener : listeners)
            listener->on_player_mat(color);
    }

    void ListenerManager::on_player_pat(const board::Color color)
    {
        for (auto& listener : listeners)
            listener->on_player_pat(color);
    }

    void ListenerManager::on_player_disqualified(const board::Color color)
    {
        for (auto& listener : listeners)
            listener->on_player_disqualified(color);
    }

    void ListenerManager::on_draw()
    {
        for (auto& listener : listeners)
            listener->on_draw();
    }

} // namespace listener
