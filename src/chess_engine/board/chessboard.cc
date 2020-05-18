
#include <strings.h>
#include <iostream>
#include "masks.hh"
#include "chessboard.hh"
#include "rule.hh"
#include "magic.hh"

namespace board
{
    void Chessboard::do_move(Move &move)
    {
        auto &rpr = getBoardRpr();

        auto dest = move.dest_pos_get();
        auto src = move.start_pos_get();

        if (move.isCaptureB())
        {
            move.setCapture(rpr.at(dest).value().first);
        }


        auto piece = std::pair<PieceType, Color>(move.piece_get(), isWhiteTurn() ? Color::WHITE : Color::BLACK);
        bool capture = move.getCapture().has_value();
        std::optional<PieceType> promotion = move.get_promotion();

        unsigned int source_int = static_cast<int>(src.file_get()) + static_cast<int>(src.rank_get()) * 8;
        unsigned int dest_int = static_cast<int>(dest.file_get()) + static_cast<int>(dest.rank_get()) * 8;

        int board_index = static_cast<int>(piece.first) + (piece.second == Color::WHITE ? 0 : 6);

        rpr.boards.at(board_index) = (rpr.boards.at(board_index) & ~(1UL << source_int));
        if (promotion.has_value())
        {
            rpr.boards.at(static_cast<int>(promotion.value()) + (piece.second == Color::WHITE ? 0 : 6)) |= 1UL << dest_int;
        } else
            rpr.boards.at(board_index) = (rpr.boards.at(board_index)) | 1UL << dest_int;
        if (capture)
        {
            auto capturePiece = std::pair<PieceType, Color>(move.getCapture().value(), isWhiteTurn() ? Color::BLACK : Color::WHITE);
            int capture_board_index = static_cast<int>(capturePiece.first) + (capturePiece.second == Color::WHITE ? 0 : 6);
            rpr.boards.at(capture_board_index) &= ~(1UL << dest_int);
        }
    }

    void Chessboard::undo_move(Move move)
    {
        auto &rpr = getBoardRpr();

        auto dest = move.dest_pos_get();
        auto src = move.start_pos_get();

        auto piece = std::pair<PieceType, Color>(move.piece_get(), isWhiteTurn() ? Color::WHITE : Color::BLACK);
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
            auto capturePiece = std::pair<PieceType, Color>(move.getCapture().value(), isWhiteTurn() ? Color::BLACK : Color::WHITE);
            int capture_board_index = static_cast<int>(capturePiece.first) + (capturePiece.second == Color::WHITE ? 0 : 6);
            rpr.boards.at(capture_board_index) |= 1UL << dest_int;
        }
    }

    std::vector<Move> Chessboard::generate_legal_moves()
    {
        auto moves = std::vector<Move>();

        std::vector<std::vector<Move>> pieceMoves;

        pieceMoves.push_back(Rule::generate_pawn_moves(*this));
        std::cout << "There are " << pieceMoves.at(0).size() << " pawn moves\n";
        pieceMoves.push_back(Rule::generate_king_moves(*this));
        std::cout << "There are " << pieceMoves.at(1).size() << " king moves\n";
        pieceMoves.push_back(Rule::generate_bishop_moves(*this));
        std::cout << "There are " << pieceMoves.at(2).size() << " bishop moves\n";
        pieceMoves.push_back(Rule::generate_rook_moves(*this));
        std::cout << "There are " << pieceMoves.at(3).size() << " rook moves\n";
        pieceMoves.push_back(Rule::generate_queen_moves(*this));
        std::cout << "There are " << pieceMoves.at(4).size() << " queen moves\n";
        pieceMoves.push_back(Rule::generate_knight_moves(*this));
        std::cout << "There are " << pieceMoves.at(5).size() << " knight moves\n";

        for (auto moveVector : pieceMoves)
            moves.insert(moves.begin(), moveVector.begin(), moveVector.end());

        for (auto i = moves.begin(); i != moves.end(); i++)
        {
            auto iCopy = *i;
            do_move(*i);
            if (is_check())
                moves.erase(i);
            undo_move(iCopy);
        }

        return moves;
    }

    Chessboard::Chessboard()
    {
        white_turn_ = true;
        white_king_castling_ = false;
        white_queen_castling_ = false;
        black_king_castling_ = false;
        black_queen_castling_ = false;
        turn_ = 1;
        last_fifty_turn_ = 50;
        boardRpr = Chessboard_rpr();
    }

    Chessboard::Chessboard(std::string str)
    {
        white_turn_ = true;
        white_king_castling_ = false;
        white_queen_castling_ = false;
        black_king_castling_ = false;
        black_queen_castling_ = false;
        turn_ = 1;
        last_fifty_turn_ = 50;
        boardRpr = Chessboard_rpr(str);
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
        return white_king_castling_;
    }

    void Chessboard::setWhiteKingCastling(bool whiteKingCastling) {
        white_king_castling_ = whiteKingCastling;
    }

    bool Chessboard::isWhiteQueenCastling() const {
        return white_queen_castling_;
    }

    void Chessboard::setWhiteQueenCastling(bool whiteQueenCastling) {
        white_queen_castling_ = whiteQueenCastling;
    }

    bool Chessboard::isBlackKingCastling() const {
        return black_king_castling_;
    }

    void Chessboard::setBlackKingCastling(bool blackKingCastling) {
        black_king_castling_ = blackKingCastling;
    }

    bool Chessboard::isBlackQueenCastling() const {
        return black_queen_castling_;
    }

    void Chessboard::setBlackQueenCastling(bool blackQueenCastling) {
        black_queen_castling_ = blackQueenCastling;
    }

    bool Chessboard::is_move_legal(Move move) {
        auto moveList = generate_legal_moves();
        for (auto moveI : moveList)
        {
            if (move == moveI)
                return true;
        }
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
}