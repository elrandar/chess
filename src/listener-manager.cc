#include "listener-manager.hh"
#include "chess_engine/board/move.hh"
#include "parsing/pgn-parser/pgn-parser.hh"
#include "chess_engine/board/chessboard.hh"
#include <vector>
#include <dlfcn.h>

namespace listener
{
    ListenerManager::ListenerManager(const std::vector<std::string>& files, board::Chessboard chessboard)
    {
        chessboard_ = chessboard;
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

    ListenerManager::~ListenerManager()
    {
        for (auto & handler : handlers)
            dlclose(handler);
    }

    board::ChessboardInterface::opt_piece_t ListenerManager::operator[](const board::Position &position) const {
        return chessboard_[position];
    }

    std::vector<board::Move> ListenerManager::pgn_to_moves(const std::string& file)
    {
        auto vect = pgn_parser::parse_pgn(file);
        std::vector<board::Move> move_list;
        move_list.reserve(vect.size());
        for (auto pgn_move : vect)
        {
            move_list.emplace_back(pgn_move.get_start(),pgn_move.get_end(),
                                   pgn_move.get_piece(), pgn_move.get_promotion(), pgn_move.get_capture());
        }
        return move_list;
    }

    bool ListenerManager::do_castling(board::Move move, board::Rank rank)
    {
        //IL FAUT CHECK SI une Case est mise en echec sur les position sur lequel le roi va passer
        if (move.dest_pos_get().file_get() == board::File::G && move.dest_pos_get().rank_get() == rank)
        {
            board::Position pos(board::File::F,rank);
            if (true){// TODO fonction pour verifier sur le roi serai en echec sur une case donnée) {
                board::Position tower_start_pos(board::File::H, rank);
                board::Position tower_dest_pos(board::File::F, rank);
                board::Move Tower_move = board::Move(tower_start_pos, tower_dest_pos, board::PieceType::ROOK);
                chessboard_.do_move(Tower_move);
                if (rank == board::Rank::ONE) {
                    on_kingside_castling(board::Color::WHITE);
                    chessboard_.setWhiteKingCastling(false);
                    chessboard_.setWhiteQueenCastling(false);
                } else{
                    on_kingside_castling(board::Color::BLACK);
                    chessboard_.setBlackKingCastling(false);
                    chessboard_.setBlackQueenCastling(false);
                }
            }
            else
            {
                return false;
            }
        }
        else if(move.dest_pos_get().file_get() == board::File::C && move.dest_pos_get().rank_get() == rank)
        {
            board::Position pos(board::File::D,rank);
            if (true) { // TODO pareil que le todo de la ligne 33 lol
                board::Position tower_start_pos(board::File::A, rank);
                board::Position tower_dest_pos(board::File::D, rank);
                board::Move Tower_move = board::Move(tower_start_pos, tower_dest_pos, board::PieceType::ROOK);
                chessboard_.do_move(Tower_move);
                if (rank == board::Rank::ONE) {
                    on_queenside_castling(board::Color::WHITE);
                    chessboard_.setWhiteQueenCastling(false);
                    chessboard_.setWhiteKingCastling(false);
                } else{
                    on_queenside_castling(board::Color::BLACK);
                    chessboard_.setBlackQueenCastling(false);
                    chessboard_.setBlackKingCastling(false);
                }
            } else{
                return false;
            }
        }
        return true;
    }


    bool ListenerManager::run_pgn(std::string pgn_path){

        std::vector<board::Move> move_list = pgn_to_moves(pgn_path);
        for(auto move: move_list)
        {
            if (!chessboard_.is_move_legal(move))
            {
                if (chessboard_.isWhiteTurn())
                {
                    disqualify(board::Color::WHITE);
                } else{
                    disqualify(board::Color::BLACK);
                }
            }
            // Execution du move
            chessboard_.do_move(move);
            if (chessboard_[move.dest_pos_get()].has_value())
            {
                on_piece_taken(chessboard_[move.dest_pos_get()]->first, move.dest_pos_get());
            }
            else
            {
                on_piece_moved(move.piece_get(), move.start_pos_get(), move.dest_pos_get());
            }

            //Gere la PROMOTION -- A TERMINER
            if (chessboard_[move.dest_pos_get()].has_value()) {
                if (move.dest_pos_get().rank_get() == board::Rank::EIGHT && chessboard_.isWhiteTurn())
                {
                    // new_piece_type -> move.get_promotion();
                    // do_promotion(new_piece_type, color);  WHITE
                    on_piece_promoted(move.get_promotion().value(),move.dest_pos_get());
                }
                else if (move.dest_pos_get().rank_get() == board::Rank::ONE && !chessboard_.isWhiteTurn())
                {
                    //do_promotion(new_piece_type, color);  BLACk
                    on_piece_promoted(move.get_promotion().value(),move.dest_pos_get());
                }
            }
            //Gere le CASTLING
            if (move.piece_get() == board::PieceType::KING)
            {
                if (chessboard_.isWhiteTurn())
                {
                    //blancs
                    if (!do_castling(move, board::Rank::ONE))
                    {
                        disqualify(board::Color::WHITE);
                    }
                    chessboard_.setWhiteKingCastling(false);
                    chessboard_.setWhiteQueenCastling(false);
                } else if (!chessboard_.isWhiteTurn()){
                    //noirs
                    if (!do_castling(move, board::Rank::EIGHT))
                    {
                        disqualify(board::Color::BLACK);
                    }
                    chessboard_.setBlackKingCastling(false);
                    chessboard_.setBlackQueenCastling(false);
                }
            }
            //Gere le Check -- A COMPLETER
            /* if ( chessboard_[move.dest_pos_get()]->first
            */
            chessboard_.setWhiteTurn(!chessboard_.isWhiteTurn());
        }
        return true;
    }



}

