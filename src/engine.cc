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

bool Engine::start_game(board::Chessboard chessboard, HandleListener handle){

    int turn = 1;
    std::vector<board::Move> move_list = get_moves(pgn_path);
    for(auto move: move_list)
    {
        if (!chessboard.is_move_legal(move))
        {
            // a faire....si le move n'est pas legal
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

        //Gere la PROMOTION
   //     if (chessboard[move.dest_post_get()])

        turn *= - 1;
        chessboard.set_turn(turn);
    }
    return true;
}
