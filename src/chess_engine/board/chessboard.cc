
#include <strings.h>
#include <iostream>
#include "masks.hh"
#include "chessboard.hh"
#include "rule.hh"

namespace board
{
    void Chessboard::do_move(Move move)
    {
        boardRpr.execute_move(move);
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

        // TODO remove moves that put the king in check

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
        if (move.dest_pos_get() == Position(56)) // FIXME de la merde pour que ca compile
        {}
        return false;
    }

    std::optional<std::pair<PieceType, Color>> Chessboard::operator[](const Position &position) const {
        return boardRpr.at(position);
    }

    Position Chessboard::king_position() {
        BitBoard kingBoard = boardRpr.get(PieceType::KING, white_turn_ ? Color::WHITE : Color::BLACK);
        return Position(BitboardOperations::bitScanForward(kingBoard));
    }

    bool Chessboard::is_check() {
        Position kingPos = king_position();
        for (auto move : generate_legal_moves())
        {
            if (move.dest_pos_get() == kingPos)
            {
                return true;
            }
        }
        return false;
    }
}