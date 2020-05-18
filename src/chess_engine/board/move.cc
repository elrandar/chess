#include "move.hh"

namespace board
{
    PieceType Move::piece_get()
    {
        return piece_;
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
        capture_b_ = false;
    }
    opt_piecetype_t Move::get_promotion()
    {
        return promotion_;
    }

    Move::Move(Position start, Position dest, PieceType pieceType, PieceType capture)
        : Move(start, dest, pieceType)
    {
        capture_ = capture;
    }

    Move::Move(Position start, Position end, PieceType pieceType, std::optional<PieceType> promotion, bool b)
        : Move(start, end, pieceType)
    {
        promotion_ = promotion;
        if (b)
        {
            capture_b_ = b;
        } else
        {
            capture_ = std::nullopt;
        }
    }

    void Move::setPromotion(const std::optional<PieceType> &promotion) {
        promotion_ = promotion;
    }

    const std::optional<PieceType> &Move::getCapture() const {
        return capture_;
    }

    bool Move::isCaptureB() const {
        return capture_b_;
    }

    void Move::setCapture(const std::optional<PieceType> &capture) {
        capture_ = capture;
    }

    bool Move::operator==(const Move &rhs) const {
        return start_pos_ == rhs.start_pos_ &&
               dest_pos_ == rhs.dest_pos_ &&
               piece_ == rhs.piece_ &&
               promotion_ == rhs.promotion_ &&
               capture_ == rhs.capture_ &&
               double_pawn_push_ == rhs.double_pawn_push_ &&
               king_castling_ == rhs.king_castling_ &&
               queen_castling_ == rhs.queen_castling_ &&
               en_passant_ == rhs.en_passant_;
    }

    bool Move::operator!=(const Move &rhs) const {
        return !(rhs == *this);
    }

    void Move::setDoublePawnPush(bool doublePawnPush) {
        double_pawn_push_ = doublePawnPush;
    }

    bool Move::isDoublePawnPush() const {
        return double_pawn_push_;
    }

    bool Move::isEnPassant() const {
        return en_passant_;
    }

    void Move::setEnPassant(bool enPassant) {
        en_passant_ = enPassant;
    }

}