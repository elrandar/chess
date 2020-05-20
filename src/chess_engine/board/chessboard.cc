
#include <strings.h>
#include <iostream>
#include "masks.hh"
#include "chessboard.hh"
#include "rule.hh"
#include "magic.hh"

namespace board
{


    void Chessboard::undo_update_castling()
    {
        if (!isWhiteTurn())
        {
            if (white_king_moved)
                white_king_moved--;
            if (white_queen_rook_moved)
                white_queen_rook_moved--;
            if (white_king_rook_moved)
                white_king_rook_moved--;
        }
        else
        {
            if (black_king_moved)
                black_king_moved--;
            if (black_queen_rook_moved)
                black_queen_rook_moved--;
            if (black_king_rook_moved)
                black_king_rook_moved--;
        }
    }

    void Chessboard::update_castling(Move &move)
    {
        bool ft_bking = false;
        bool ft_wking = false;
        bool ft_wrook_queen = false;
        bool ft_brook_queen = false;
        bool ft_wrook_king = false;
        bool ft_brook_king = false;

        auto piece = move.piece_get();
        if (piece == PieceType::KING)
        {
            isWhiteTurn() ? ft_wking = true : ft_bking = true;
            //first time rook move if it is with a castling
            if (move.isKingCastling())
                isWhiteTurn() ? ft_wrook_king = true : ft_brook_king = true;
            else if (move.isQueenCastling())
                isWhiteTurn() ? ft_wrook_queen = true : ft_brook_queen = true;
        }
        else if (piece == PieceType::ROOK) //first time rook move
        {
            if (isWhiteTurn())
            {
                if (move.start_pos_get() == Position(7))
                {
                    ft_wrook_king = true;
                }
                else if (move.start_pos_get()  == Position(0))
                {
                    ft_wrook_queen = true;
                }
            }
            else
            {
                if (move.start_pos_get() == Position(63))
                {
                    ft_brook_king = true;
                }
                else if (move.start_pos_get()  == Position(56))
                {
                    ft_brook_queen = true;
                }
            }
        }
        //update number of move after rook/king moved
        if (isWhiteTurn())
        {
            if (white_king_moved || ft_wking)
                white_king_moved++;
            if (white_queen_rook_moved || ft_wrook_queen)
                white_queen_rook_moved++;
            if (white_king_rook_moved || ft_wrook_king)
                white_king_rook_moved++;
        }
        else
        {
            if (black_king_moved || ft_bking)
                black_king_moved++;
            if (black_queen_rook_moved || ft_brook_queen)
                black_queen_rook_moved++;
            if (black_king_rook_moved || ft_brook_king)
                black_king_rook_moved++;
        }
    }

    void Chessboard::do_castling(Move &move)
    {
        auto king_board = isWhiteTurn() ? 5 : 11;
        auto rook_board = isWhiteTurn() ? 1 : 7;
        auto king_side_rook_mask = isWhiteTurn() ? ~128UL : ~(1UL << 63UL);
        auto king_side_rook_set = isWhiteTurn() ? (1UL << 5UL) : (1UL << 61UL);
        auto queen_side_rook_mask = isWhiteTurn() ? ~1UL : ~(1UL << 56UL);
        auto queen_side_rook_set = isWhiteTurn() ? (1UL << 3UL) : (1UL << 59UL);

        if (move.isKingCastling())
        {
            boardRpr.boards[king_board] <<= 2UL;
            boardRpr.boards[rook_board] &= king_side_rook_mask;
            boardRpr.boards[rook_board] |= king_side_rook_set;
        }
        else
        {
            boardRpr.boards[king_board] >>= 2UL;
            boardRpr.boards[rook_board] &= queen_side_rook_mask;
            boardRpr.boards[rook_board] |= queen_side_rook_set;
        }
    }
    void Chessboard::do_move(Move &move)
    {
        if (move.isKingCastling() || move.isQueenCastling())
        {
            do_castling(move);
            update_castling(move);
            en_passant_.push(0ul);
            setWhiteTurn(!isWhiteTurn());
            return;
        }
        auto &rpr = getBoardRpr();

        auto dest = move.dest_pos_get();
        auto src = move.start_pos_get();

        auto piece = std::pair<PieceType, Color>(move.piece_get(), isWhiteTurn() ? Color::WHITE : Color::BLACK);
        bool capture = move.getCapture().has_value();
        std::optional<PieceType> promotion = move.get_promotion();

        update_castling(move);

        unsigned int source_int = static_cast<int>(src.file_get()) + static_cast<int>(src.rank_get()) * 8;
        unsigned int dest_int = static_cast<int>(dest.file_get()) + static_cast<int>(dest.rank_get()) * 8;
        int board_index = static_cast<int>(piece.first) + (piece.second == Color::WHITE ? 0 : 6);

        rpr.boards.at(board_index) = (rpr.boards.at(board_index) & ~(1UL << source_int));
        // PROMOTION
        if (promotion.has_value())
        {
            rpr.boards.at(static_cast<int>(promotion.value()) + (piece.second == Color::WHITE ? 0 : 6)) |= 1UL << dest_int;
        } else
            rpr.boards.at(board_index) = (rpr.boards.at(board_index)) | 1UL << dest_int;
        // CAPTURE
        if (capture)
        {
            auto capturePiece = std::pair<PieceType, Color>(move.getCapture().value(), isWhiteTurn() ? Color::BLACK : Color::WHITE);
            int capture_board_index = static_cast<int>(capturePiece.first) + (capturePiece.second == Color::WHITE ? 0 : 6);
            if (move.isEnPassant())
                rpr.boards.at(capture_board_index) &= ~(1UL << (dest_int + (isWhiteTurn() ? -8 : 8)));
            else
                rpr.boards.at(capture_board_index) &= ~(1UL << dest_int);
        }
        // DOUBLE PUSH (EN PASSANT)
        if (move.isDoublePawnPush())
        {
            en_passant_.push(isWhiteTurn() ? 1ul << (dest_int - 8) : 1ul << (dest_int + 8));
        } else
            en_passant_.push(0ul);
        setWhiteTurn(!isWhiteTurn());
    }


    void Chessboard::undo_castling(Move &move)
    {
        !isWhiteTurn() ? white_king_moved = 0 : black_king_moved = 0;

        auto king_board = !isWhiteTurn() ? 5 : 11;
        auto rook_board = !isWhiteTurn() ? 1 : 7;
        auto king_side_rook_mask = !isWhiteTurn() ? ~(1UL << 5UL) : ~(1UL << 61UL);
        auto queen_side_rook_mask = !isWhiteTurn() ? ~(1UL << 3UL) : ~(1UL << 59UL);
        auto king_side_rook_set = !isWhiteTurn() ? ~128UL : ~(1UL << 63UL);
        auto queen_side_rook_set = !isWhiteTurn() ? 1UL : (1UL << 56UL);

        if (move.isKingCastling())
        {
            !isWhiteTurn() ? white_king_rook_moved = 0 : black_king_rook_moved = 0;
            boardRpr.boards[king_board] >>= 2UL;
            boardRpr.boards[rook_board] &= king_side_rook_mask;
            boardRpr.boards[rook_board] |= king_side_rook_set;
        }
        else
        {
            !isWhiteTurn() ? white_queen_rook_moved = 0 : black_queen_rook_moved = 0;
            boardRpr.boards[king_board] <<= 2UL;
            boardRpr.boards[rook_board] &= queen_side_rook_mask;
            boardRpr.boards[rook_board] |= queen_side_rook_set;
        }
    }
    void Chessboard::undo_move(Move move)
    {
        getEnPassant().pop();
        if (move.isQueenCastling() || move.isKingCastling())
        {
            undo_castling(move);
            setWhiteTurn(!isWhiteTurn());
            return;
        }
        undo_update_castling();
        auto &rpr = getBoardRpr();

        auto dest = move.dest_pos_get();
        auto src = move.start_pos_get();

        auto piece = std::pair<PieceType, Color>(move.piece_get(), isWhiteTurn() ? Color::BLACK : Color::WHITE);
        bool capture = move.getCapture().has_value();
        std::optional<PieceType> promotion = move.get_promotion();

        unsigned int source_int = static_cast<int>(src.file_get()) + static_cast<int>(src.rank_get()) * 8;
        unsigned int dest_int = static_cast<int>(dest.file_get()) + static_cast<int>(dest.rank_get()) * 8;

        int board_index = static_cast<int>(piece.first) + (piece.second == Color::WHITE ? 0 : 6);

        rpr.boards.at(board_index) = (rpr.boards.at(board_index)) | 1UL << source_int;
        if (promotion.has_value())
        {
            rpr.boards.at(static_cast<int>(promotion.value()) + (piece.second == Color::WHITE ? 0 : 6)) &= ~(1UL << dest_int);
        } else
            rpr.boards.at(board_index) = (rpr.boards.at(board_index) & ~(1UL << dest_int));
        if (capture)
        {
            auto capturePiece = std::pair<PieceType, Color>(move.getCapture().value(), isWhiteTurn() ? Color::WHITE : Color::BLACK);
            int capture_board_index = static_cast<int>(capturePiece.first) + (capturePiece.second == Color::WHITE ? 0 : 6);
            if (move.isEnPassant())
            {
                rpr.boards.at(capture_board_index) |= (1UL << (dest_int + (!isWhiteTurn() ? -8 : 8)));
            }
            else
                rpr.boards.at(capture_board_index) |= 1UL << dest_int;
        }
        setWhiteTurn(!isWhiteTurn());
    }

    std::vector<Move> Chessboard::generate_legal_moves()
    {
        auto moves = std::vector<Move>();

        std::vector<std::vector<Move>> pieceMoves;

        pieceMoves.push_back(Rule::generate_pawn_moves(*this));
        //std::cout << "There are " << pieceMoves.at(0).size() << " pawn moves\n";
        pieceMoves.push_back(Rule::generate_king_moves(*this));
//            std::cout << "There are " << pieceMoves.at(1).size() << " king moves\n";
        pieceMoves.push_back(Rule::generate_bishop_moves(*this));
//            std::cout << "There are " << pieceMoves.at(2).size() << " bishop moves\n";
        pieceMoves.push_back(Rule::generate_rook_moves(*this));
//            std::cout << "There are " << pieceMoves.at(3).size() << " rook moves\n";
        pieceMoves.push_back(Rule::generate_queen_moves(*this));
//            std::cout << "There are " << pieceMoves.at(4).size() << " queen moves\n";
        pieceMoves.push_back(Rule::generate_knight_moves(*this));
//            std::cout << "There are " << pieceMoves.at(5).size() << " knight moves\n";

        for (auto moveVector : pieceMoves)
            moves.insert(moves.begin(), moveVector.begin(), moveVector.end());

        auto keepList = std::vector<Move>();
        for (auto i = moves.begin(); i != moves.end(); i++)
        {
            auto iCopy = *i;
            do_move(iCopy); // changes the color
            setWhiteTurn(!isWhiteTurn()); // We want to check that the king from the color that did the move is in CHECK
            if (!is_check())
                keepList.push_back(*i);
            setWhiteTurn(!isWhiteTurn());
            undo_move(iCopy); // restore the color
        }

        //add castling moves to the vector
        generate_castling(keepList);

        return keepList;
    }

    void Chessboard::generate_castling(std::vector<Move> &moves)
    {
        auto king_castling = (isWhiteTurn() ? !white_king_rook_moved && !white_king_moved: !black_king_rook_moved && !black_king_moved);
        auto queen_castling = (isWhiteTurn() ? !white_queen_rook_moved && !white_king_moved: !black_queen_rook_moved && !black_king_moved);
        auto dest_queen_castling = isWhiteTurn() ? Position(2) : Position(58);
        auto dest_king_castling = isWhiteTurn() ? Position(6) : Position(62);
        auto src_castling = isWhiteTurn() ? Position(4) : Position(60);
        auto mask_king_side_occupied = isWhiteTurn() ? 0x60UL : 0x6000000000000000UL;
        auto mask_queen_side_occupied = isWhiteTurn() ? 0xeUL : 0xe00000000000000UL;
        auto index_king_side = isWhiteTurn() ? 5 : 61;
        auto index_queen_side = isWhiteTurn() ? 2 : 58;
        auto attacker_color = isWhiteTurn() ? Color::BLACK : Color::WHITE;

        if (king_castling)
        {
            if ((boardRpr.occupied() & mask_king_side_occupied) == 0UL)
            {
                if (!is_sq_attacked_by_color(index_king_side, attacker_color)
                    && !is_sq_attacked_by_color(index_king_side + 1 , attacker_color))
                {
                    auto castling = Move(src_castling, dest_king_castling, PieceType::KING);
                    castling.setKingCastling(true);
                    moves.push_back(castling);
//                    std::cout << "king castling possible !\n";
                }
            }
        }
        if (queen_castling)
        {
            if ((boardRpr.occupied() & mask_queen_side_occupied) == 0UL)
            {
                if (!is_sq_attacked_by_color(index_queen_side, attacker_color)
                    && !is_sq_attacked_by_color(index_queen_side + 1 , attacker_color))
                {
                    auto castling = Move(src_castling, dest_queen_castling, PieceType::KING);
                    castling.setQueenCastling(true);
                    moves.push_back(castling);
//                    std::cout << "queen castling possible !\n";
                }
            }
        }

    }


    Chessboard::Chessboard()
    {
        white_turn_ = true;
        white_king_rook_moved = 0;
        white_queen_rook_moved = 0;
        black_king_rook_moved = 0;
        black_queen_rook_moved = 0;
        white_king_moved = 0;
        black_king_moved = 0;
        turn_ = 1;
        last_fifty_turn_ = 50;
        boardRpr = Chessboard_rpr();
        en_passant_ = std::stack<BitBoard>();
        en_passant_.push(0ul);
    }

    Chessboard::Chessboard(std::string str)
    {
        white_turn_ = true;
        white_king_rook_moved = 0;
        white_queen_rook_moved = 0;
        black_king_rook_moved = 0;
        black_queen_rook_moved = 0;
        white_king_moved = 0;
        black_king_moved = 0;
        turn_ = 1;
        last_fifty_turn_ = 50;
        boardRpr = Chessboard_rpr(str);
        en_passant_ = std::stack<BitBoard>();
        en_passant_.push(0ul);
    }

    Chessboard_rpr& Chessboard::getBoardRpr() {
        return boardRpr;
    }

    bool Chessboard::isWhiteTurn() const {
        return white_turn_;
    }

    void Chessboard::setWhiteTurn(bool whiteTurn) {
        white_turn_ = whiteTurn;
    }

    bool Chessboard::isWhiteKingCastling() const {
        return white_king_rook_moved;
    }

    void Chessboard::setWhiteKingCastling(bool whiteKingCastling) {
        white_king_rook_moved = whiteKingCastling;
    }

    bool Chessboard::isWhiteQueenCastling() const {
        return white_queen_rook_moved;
    }

    void Chessboard::setWhiteQueenCastling(bool whiteQueenCastling) {
        white_queen_rook_moved = whiteQueenCastling;
    }

    bool Chessboard::isBlackKingCastling() const {
        return black_king_rook_moved;
    }

    void Chessboard::setBlackKingCastling(bool blackKingCastling) {
        black_king_rook_moved = blackKingCastling;
    }

    bool Chessboard::isBlackQueenCastling() const {
        return black_queen_rook_moved;
    }

    void Chessboard::setBlackQueenCastling(bool blackQueenCastling) {
        black_queen_rook_moved = blackQueenCastling;
    }

    bool Chessboard::is_move_legal(Move move) {
        auto moveList = generate_legal_moves();
        for (auto legalMove : moveList)
            if (move == legalMove)
                return true;
        return false;
    }

    std::optional<std::pair<PieceType, Color>> Chessboard::operator[](const Position &position) const {
        return boardRpr.at(position);
    }

    Position Chessboard::king_position() {
        BitBoard kingBoard = boardRpr.get(PieceType::KING, white_turn_ ? Color::WHITE : Color::BLACK);
        return Position(BitboardOperations::bitScanForward(kingBoard));
    }

    bool Chessboard::is_sq_attacked_by_color(int sq, Color color)
    {
        Color defendingColor = color == Color::WHITE ? Color::BLACK : Color::WHITE;
        BitBoard pawns = boardRpr.get(PieceType::PAWN, color);
        BitBoard knights = boardRpr.get(PieceType::KNIGHT, color);
        BitBoard king = boardRpr.get(PieceType::KING, color);
        BitBoard bishop_queens = boardRpr.get(PieceType::BISHOP, color) | boardRpr.get(PieceType::QUEEN, color);
        BitBoard rook_queens = boardRpr.get(PieceType::ROOK, color) | boardRpr.get(PieceType::QUEEN, color);
        if (Masks::pawn_attacks(defendingColor, sq) & pawns)
            return true;
        if (Masks::knight_attacks(sq) & knights)
            return true;
        if (Masks::king_attacks(sq) & king)
            return true;
        if (magic::attack_bishop(boardRpr.occupied(), sq) & bishop_queens)
            return true;
        if (magic::attack_rook(boardRpr.occupied(), sq) & rook_queens)
            return true;
        return false;
    }


    bool Chessboard::is_check() {
        Position kingPos = king_position();
        return is_sq_attacked_by_color(static_cast<int>(kingPos.file_get()) + 8 * static_cast<int>(kingPos.rank_get()),
                isWhiteTurn() ? Color::BLACK : Color::WHITE);
    }

    std::stack<BitBoard> & Chessboard::getEnPassant() {
        return en_passant_;
    }

    bool Chessboard::is_pat() {
        return !is_check() && generate_legal_moves().empty();
    }

    bool Chessboard::is_checkmate() {
        return is_check() && generate_legal_moves().empty();
    }

    bool Chessboard::is_draw() {
        return is_pat() || last_fifty_turn_ == 0;
    }

}