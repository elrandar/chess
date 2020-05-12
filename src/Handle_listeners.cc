#include "Handle_listeners.hh"
#include "move.hh"
#include "listener.hh"
#include <iostream>
#include <dlfcn.h>

HandleListener::HandleListener(std::vector<std::string> files)
{
    listeners = std::vector<Listener*>();
    handlers = std::vector<void*>();

    for (auto &path : files) {
        void *handle = dlopen(path.c_str(), RTLD_NOW);
        if (handle == nullptr) {
            perror("dlopen failed");
        }
        void *symbol = dlsym(handle, "listener_create");
        handlers.push_back(handle);
        listeners.push_back(reinterpret_cast<Listener *(*)()>(symbol)());
    }
}

HandleListener::~HandleListener()
{
    for (auto & handler : handlers)
        dlclose(handler);
}

void HandleListener::register_board(const board::ChessboardInterface& board_interface)
{
    for (auto & listener : listeners)
        listener->register_board(board_interface);
}


void HandleListener::on_game_finished()
{
    for (auto & listener : listeners)
        listener->on_game_finished();
}

void HandleListener::on_piece_moved(const board::PieceType piece,
                                    const board::Position& from,
                                    const board::Position& to)
{
    for (auto & listener : listeners)
        listener->on_piece_moved(piece, from, to);
}

void HandleListener::on_piece_taken(const board::PieceType piece,
                                    const board::Position& at)
{
    for (auto & listener : listeners)
        listener->on_piece_taken(piece, at);
}

void HandleListener::on_piece_promoted(const board::PieceType piece,
                                       const board::Position& at)
{
    for (auto & listener : listeners)
    {
        listener->on_piece_promoted(piece,at);
    }                                       
}

void HandleListener::on_kingside_castling(const board::Color color)
{
    for (auto & listener : listeners)
        listener->on_kingside_castling(color);
}

void HandleListener::on_queenside_castling(const board::Color color)
{
    for (auto & listener : listeners)
        listener->on_queenside_castling(color);
}

void HandleListener::on_player_check(const board::Color color)
{
    for (auto & listener : listeners)
        listener->on_player_check(color);
}

void HandleListener::on_player_mat(const board::Color color)
{
    for (auto & listener : listeners)
        listener->on_player_mat(color);
}

void HandleListener::on_player_pat(const board::Color color)
{
    for (auto & listener : listeners)
        listener->on_player_pat(color);
}


void HandleListener::on_player_disqualified(const board::Color color)
{
    for (auto & listener : listeners)
        listener->on_player_disqualified(color);
}

void HandleListener::on_draw()
{
    for (auto & listener : listeners)
        listener->on_draw();
}