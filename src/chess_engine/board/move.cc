#include "move.hh"

#include <iostream>

namespace board
{
    PieceType Move::piece_get()
    {
        return piece_.value();
    }
    Position Move::start_pos_get()
    {
        return start_pos_;
    }
    Position Move::dest_pos_get()
    {
        return dest_pos_;
    }

    Move::Move(Position start, Position dest, PieceType pieceType)
        : start_pos_(start)
        , dest_pos_(dest)
        , piece_(pieceType)
    {
        double_pawn_push_ = false;
        king_castling_ = false;
        queen_castling_ = false;
        en_passant_ = false;
        promotion_ = std::nullopt;
        capture_ = std::nullopt;
    }
    opt_piecetype_t Move::get_promotion()
    {
        return promotion_;
    }

    Move::Move(Position start, Position dest, PieceType pieceType,
               PieceType capture)
        : Move(start, dest, pieceType)
    {
        capture_ = capture;
    }

    Move::Move(Position start, Position end, PieceType pieceType,
               std::optional<PieceType> promotion)
        : Move(start, end, pieceType)
    {
        promotion_ = promotion;
    }

    void Move::setPromotion(const std::optional<PieceType>& promotion)
    {
        promotion_ = promotion;
    }

    const std::optional<PieceType>& Move::getCapture() const
    {
        return capture_;
    }

    void Move::setCapture(const std::optional<PieceType>& capture)
    {
        capture_ = capture;
    }

    void Move::setDoublePawnPush(bool doublePawnPush)
    {
        double_pawn_push_ = doublePawnPush;
    }

    bool Move::isDoublePawnPush() const
    {
        return double_pawn_push_;
    }

    bool Move::isEnPassant() const
    {
        return en_passant_;
    }

    void Move::setEnPassant(bool enPassant)
    {
        en_passant_ = enPassant;
    }

    void Move::setKingCastling(bool kingCastling)
    {
        king_castling_ = kingCastling;
    }

    void Move::setQueenCastling(bool queenCastling)
    {
        queen_castling_ = queenCastling;
    }

    bool Move::isKingCastling() const
    {
        return king_castling_;
    }

    bool Move::isQueenCastling() const
    {
        return queen_castling_;
    }

    bool Move::operator==(const Move& rhs) const
    {
        return start_pos_ == rhs.start_pos_ && dest_pos_ == rhs.dest_pos_
            && piece_ == rhs.piece_ && promotion_ == rhs.promotion_
            && capture_ == rhs.capture_
            && double_pawn_push_ == rhs.double_pawn_push_
            && king_castling_ == rhs.king_castling_
            && queen_castling_ == rhs.queen_castling_
            && en_passant_ == rhs.en_passant_;
    }

    bool Move::operator!=(const Move& rhs) const
    {
        return !(rhs == *this);
    }

    void Move::print()
    {
        std::cout << (char)('a' + static_cast<int>(start_pos_.file_get()))
                  << static_cast<int>(start_pos_get().rank_get()) + 1;
        std::cout << (char)('a' + static_cast<int>(dest_pos_.file_get()))
                  << static_cast<int>(dest_pos_get().rank_get()) + 1 << '\n';
    }

    std::string Move::toString()
    {
        auto out = std::string();
        out.push_back(('a' + static_cast<int>(start_pos_.file_get())));
        out.push_back('0' + static_cast<int>(start_pos_get().rank_get()) + 1);
        out.push_back(('a' + static_cast<int>(dest_pos_.file_get())));
        out.push_back('0' + static_cast<int>(dest_pos_get().rank_get()) + 1);
        if (promotion_.has_value())
        {
            std::array<char, 4> promotionPieces = {'q', 'r', 'b', 'n'};
            out.push_back(
                promotionPieces.at(static_cast<int>(promotion_.value())));
        }
        return out;
    }

    Move::Move(Position start, Position dest)
        : start_pos_(start)
        , dest_pos_(dest)
    {
        piece_ = std::nullopt;
        promotion_ = std::nullopt;
        capture_ = std::nullopt;
        double_pawn_push_ = false;
        king_castling_ = false;
        queen_castling_ = false;
        en_passant_ = false;
    }

} // namespace board