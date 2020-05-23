#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include "../../parsing/perft-parser/fen-object.hh"
#include "bitboard-operations.hh"
#include "color.hh"
#include "move.hh"

namespace board
{
    class Chessboard_rpr
    {
    private:
    public:
        explicit Chessboard_rpr(perft_parser::FenObject fenObject);

        std::array<BitBoard, 12> boards{};
        BitBoard occupied;
        BitBoard WhitePieces;
        BitBoard BlackPieces;
        BitBoard computeOccupied();
        BitBoard computeWhitePieces();
        BitBoard computeBlackPieces();
        void update();
        BitBoard get(PieceType pieceType, Color color);
        static std::string bitBoardPrint(BitBoard bitBoard);
        Chessboard_rpr();
        void print();
        [[nodiscard]] std::optional<std::pair<PieceType, Color>>
        at(Position pos) const;
    };
} // namespace board