#include "engine.hh"
#include <vector>
#include <iostream>

std::vector<board::Move> Engine::get_moves(const std::string& file)
{
    auto vect = pgn_parser::parse_pgn(file);
    std::vector<board::Move> move_list;
    move_list.reserve(vect.size());
    for (auto pgn_move : vect)
    {
        move_list.emplace_back(pgn_move.get_start(),pgn_move.get_end(),
                pgn_move.get_piece(), pgn_move.get_promotion().value(), pgn_move.get_capture());
    }
    return move_list;
}

void Engine::disqualify(HandleListener handle,board::Color color)
{
    handle.on_player_disqualified(color);
    handle.on_game_finished();
}

bool Engine::do_castling(board::Chessboard chessboard, HandleListener handle, board::Move move, board::Rank rank)
{
    //IL FAUT CHECK SI une Case est mise en echec sur les position sur lequel le roi va passer
    if (move.dest_pos_get().file_get() == board::File::G && move.dest_pos_get().rank_get() == rank)
    {
        board::Position pos(board::File::F,rank);
        if (!chessboard.is_check(pos)) {
            board::Position tower_start_pos(board::File::H, rank);
            board::Position tower_dest_pos(board::File::F, rank);
            board::Move Tower_move = board::Move(tower_start_pos, tower_dest_pos, board::PieceType::ROOK);
            chessboard.do_move(Tower_move);
            if (rank == board::Rank::ONE) {
                handle.on_kingside_castling(board::Color::WHITE);
                chessboard.set_white_castling(false);
            } else{
                handle.on_kingside_castling(board::Color::BLACK);
                chessboard.set_black_castling(false);
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
        if (!chessboard.is_check(pos)) {
            board::Position tower_start_pos(board::File::A, rank);
            board::Position tower_dest_pos(board::File::D, rank);
            board::Move Tower_move = board::Move(tower_start_pos, tower_dest_pos, board::PieceType::ROOK);
            chessboard.do_move(Tower_move);
            if (rank == board::Rank::ONE) {
                handle.on_queenside_castling(board::Color::WHITE);
                chessboard.set_white_castling(false);
            } else{
                handle.on_queenside_castling(board::Color::BLACK);
                chessboard.set_black_castling(false);
            }
        } else{
            return false;
        }
    }
    return true;
}
bool Engine::start_game(board::Chessboard chessboard, HandleListener handle){

    int turn = 1;
    // 1 -> tour des blancs
    // -1 -> tour des noirs
    std::vector<board::Move> move_list = get_moves(pgn_path);
    for(auto move: move_list)
    {
        if (!chessboard.is_move_legal(move))
        {
            if (chessboard.get_turn() == 1)
            {
                disqualify(handle,board::Color::WHITE);
            } else{
                disqualify(handle,board::Color::BLACK);
            }
        }
        // Execution du move
        chessboard.do_move(move);
        if (chessboard[move.dest_pos_get()].has_value())
        {
            handle.on_piece_taken(chessboard[move.dest_pos_get()]->first, move.dest_pos_get());
        }
        else
        {
            handle.on_piece_moved(move.piece_get(), move.start_pos_get(), move.dest_pos_get());
        }

        //Gere la PROMOTION -- A TERMINER
        if (chessboard[move.dest_pos_get()].has_value()) {
            if (chessboard[move.dest_pos_get()].rank_get() == board::Rank::EIGHT && chessboard.get_turn() == 1)
            {
                // new_piece_type -> move.get_promotion();
                // do_promotion(new_piece_type, color);  WHITE
                handle.on_piece_promoted(move.get_promotion().value(),move.dest_pos_get());
            }
            else if (chessboard[move.dest_pos_get()].rank_get() == board::Rank::ONE && chessboard.get_turn() == -1u)
            {
                //do_promotion(new_piece_type, color);  BLACk
                handle.on_piece_promoted(move.get_promotion().value(),move.dest_pos_get());
            }
        }
        //Gere le CASTLING
        if (move.piece_get() == board::PieceType::KING)
        {
            if (chessboard.get_turn() == 1)
            {
                //blancs
                if (!do_castling(chessboard,handle,move,board::Rank::ONE))
                {
                    disqualify(handle,board::Color::WHITE);
                }
                chessboard.set_white_castling(false);
            } else if (chessboard.get_turn() == -1u){
                //noirs
                if (!do_castling(chessboard,handle,move,board::Rank::EIGHT))
                {
                    disqualify(handle,board::Color::BLACK);
                }
                chessboard.set_black_castling(false);
            }
        }
        //Gere le Check -- A COMPLETER
       /* if ( chessboard[move.dest_pos_get()]->first
*/
        turn *= - 1;
        chessboard.set_turn(turn);
    }
    return true;
}
