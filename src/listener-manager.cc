#include "listener-manager.hh"
#include "chess_engine/board/move.hh"
#include "parsing/pgn-parser/pgn-parser.hh"
#include "chess_engine/board/chessboard.hh"
#include <utility>
#include <vector>
#include <dlfcn.h>

namespace listener
{
    ListenerManager::ListenerManager(const std::vector<std::string>& files, board::Chessboard &chessboard)
    {
        chessboard_ = std::move(chessboard);
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
        for (auto listener : listeners)
            delete listener;
        for (auto & handler : handlers)
            dlclose(handler);
    }

    board::ChessboardInterface::opt_piece_t ListenerManager::operator[](const board::Position &position) const {
        return chessboard_[position];
    }

    int ListenerManager::is_pgn_move_castling(board::PgnMove pgnMove)
    {
        if (pgnMove == board::PgnMove::generate_castling(false, board::Color::WHITE))
            return 1;
        if (pgnMove == board::PgnMove::generate_castling(true, board::Color::WHITE))
            return 2;
        if (pgnMove == board::PgnMove::generate_castling(false, board::Color::BLACK))
            return 1;
        if (pgnMove == board::PgnMove::generate_castling(true, board::Color::BLACK))
            return 2;
        return 0;
    }

    board::Move ListenerManager::pgnMoveToMove(board::PgnMove pgnMove, board::Chessboard chessboard)
    {
        auto move = board::Move(pgnMove.get_start(), pgnMove.get_end(), pgnMove.get_piece(),
                pgnMove.get_promotion());
        // Set capture
        if (pgnMove.get_capture())
        {
            move.setCapture(chessboard[move.dest_pos_get()].value().first);
        }
        // Set castling
        auto castling = is_pgn_move_castling(pgnMove);
        if (castling == 1)
            move.setKingCastling(true);
        else if (castling == 2)
            move.setQueenCastling(true);
        // Set double pawn
        if (pgnMove.get_piece() == board::PieceType::PAWN &&
                pgnMove.get_end() == board::Position(pgnMove.get_start().file_get(),
                        chessboard.isWhiteTurn() ? static_cast<board::Rank>(static_cast<int>(pgnMove.get_start().rank_get()) + 2)
                                                 : static_cast<board::Rank>(static_cast<int>(pgnMove.get_start().rank_get()) - 2)))
        {
            move.setDoublePawnPush(true);
        }
        // Set en_passant
        auto en_passant = chessboard.getEnPassant().top();
        auto bitSetEnPassant = board::BitboardOperations::bitScanForward(en_passant);
        if (bitSetEnPassant != -1 && move.dest_pos_get() == board::Position(bitSetEnPassant) && pgnMove.get_capture())
            move.setEnPassant(true);
        // ======
        return move;
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

    bool ListenerManager::run_pgn(const std::string& pgn_path)
    {
        // Parse the pgn_file to build a vector of pgn moves
        auto pgnMoves = pgn_parser::parse_pgn(pgn_path);
        for (auto pgnMove : pgnMoves) {
            // Complete the fields of the pgnMove to turn it into a Move
            auto move = pgnMoveToMove(pgnMove, chessboard_);

            // Check that the move is legal
            if (!chessboard_.is_move_legal(move)) {
                if (chessboard_.isWhiteTurn()) {
                    disqualify(board::Color::WHITE);
                    break;
                } else {
                    disqualify(board::Color::BLACK);
                    break;
                }
            }

            board::Color moveColor = chessboard_.isWhiteTurn() ? board::Color::WHITE : board::Color::BLACK;
            board::Color enemyColor = moveColor == board::Color::WHITE ? board::Color::BLACK : board::Color::WHITE;
            // Check le castling
            if (move.isQueenCastling() || move.isKingCastling())
            {
                on_piece_moved(move.piece_get(), move.start_pos_get(), move.dest_pos_get());
                if (move.isQueenCastling())
                    on_queenside_castling(moveColor);
                else
                    on_kingside_castling(moveColor);
            }
            else // Pas un castling
            {
                on_piece_moved(move.piece_get(), move.start_pos_get(), move.dest_pos_get());
                auto pieceTaken = move.getCapture();
                if (pieceTaken.has_value()) // Check capture
                    on_piece_taken(pieceTaken.value(), move.dest_pos_get());
                auto piecePromoted = move.get_promotion();
                if (piecePromoted.has_value())
                    on_piece_promoted(piecePromoted.value(), move.dest_pos_get());
            }

            // Move is executed, active side changes
            chessboard_.do_move(move);

            if (chessboard_.is_checkmate()) //Checks if the other player (the one that has not played the move) is checkmate
            {
                on_player_mat(enemyColor);
                on_game_finished();
                break;
            }
            if (chessboard_.is_check())
            {
                on_player_check(enemyColor);
            }
            if (chessboard_.is_pat())
            {
                on_player_pat(enemyColor);
                on_draw();
                on_game_finished();
                break;
            }
            if (chessboard_.is_draw())
            {
                on_draw();
                on_game_finished();
                break;
            }
            chessboard_.getBoardRpr().print();
            // Continue, there is no draw, the game is not finished
        }
        return true;
    }

//    bool ListenerManager::run_pgn(std::string pgn_path){
//
//        std::vector<board::Move> move_list = pgnMoveToMove(pgn_path, board::Chessboard());
//        auto pgnMoves = pgn_parser::parse_pgn(pgn_path);
//        for(auto move: move_list)
//        {
//            if (!chessboard_.is_move_legal(move))
//            {
//                if (chessboard_.isWhiteTurn())
//                {
//                    disqualify(board::Color::WHITE);
//                } else{
//                    disqualify(board::Color::BLACK);
//                }
//            }
//            // Execution du move
//            chessboard_.do_move(move);
//            if (chessboard_[move.dest_pos_get()].has_value())
//            {
//                on_piece_taken(chessboard_[move.dest_pos_get()]->first, move.dest_pos_get());
//            }
//            else
//            {
//                on_piece_moved(move.piece_get(), move.start_pos_get(), move.dest_pos_get());
//            }
//
//            //Gere la PROMOTION -- A TERMINER
//            if (chessboard_[move.dest_pos_get()].has_value()) {
//                if (move.dest_pos_get().rank_get() == board::Rank::EIGHT && chessboard_.isWhiteTurn())
//                {
//                    // new_piece_type -> move.get_promotion();
//                    // do_promotion(new_piece_type, color);  WHITE
//                    on_piece_promoted(move.get_promotion().value(),move.dest_pos_get());
//                }
//                else if (move.dest_pos_get().rank_get() == board::Rank::ONE && !chessboard_.isWhiteTurn())
//                {
//                    //do_promotion(new_piece_type, color);  BLACk
//                    on_piece_promoted(move.get_promotion().value(),move.dest_pos_get());
//                }
//            }
//            //Gere le CASTLING
//            if (move.piece_get() == board::PieceType::KING)
//            {
//                if (chessboard_.isWhiteTurn())
//                {
//                    //blancs
//                    if (!do_castling(move, board::Rank::ONE))
//                    {
//                        disqualify(board::Color::WHITE);
//                    }
//                    chessboard_.setWhiteKingCastling(false);
//                    chessboard_.setWhiteQueenCastling(false);
//                } else if (!chessboard_.isWhiteTurn()){
//                    //noirs
//                    if (!do_castling(move, board::Rank::EIGHT))
//                    {
//                        disqualify(board::Color::BLACK);
//                    }
//                    chessboard_.setBlackKingCastling(false);
//                    chessboard_.setBlackQueenCastling(false);
//                }
//            }
//            //Gere le Check -- A COMPLETER
//            /* if ( chessboard_[move.dest_pos_get()]->first
//            */
//            chessboard_.setWhiteTurn(!chessboard_.isWhiteTurn());
//        }
//        return true;
//    }



}

