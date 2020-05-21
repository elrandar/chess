#include "evaluation.hh"
#include "../board/bitboard-operations.hh"

namespace ai
{

    Evaluation::Evaluation(Chessboard &chessboard)
        : chessboard_(chessboard)
    {
        WisolatedPawns = 0;
        WblockedPawns = 0;
        WdoubledPawns = 0;
        BisolatedPawns = 0;
        BblockedPawns = 0;
        BdoubledPawns = 0;

        chessboard_ = chessboard;
    }

    float Evaluation::rate_chessboard(Color side)
    {
        double king_factor = 2000;
        double queen_factor = 90;
        double rook_factor = 50;
        double bishop_knight_factor = 30;
        double pawn_factor = 10;
        double double_blocked_isolated_pawn_factor = 5;

        double king_value = king_factor * (count_pieces(board::PieceType::KING, Color::WHITE)
                                - count_pieces(board::PieceType::KING, board::Color::BLACK));
        double queen_value = queen_factor * (count_pieces(board::PieceType::QUEEN, board::Color::WHITE)
                                - count_pieces(board::PieceType::QUEEN, board::Color::BLACK));
        double rook_value = rook_factor * (count_pieces(board::PieceType::ROOK, board::Color::WHITE)
                                           - count_pieces(board::PieceType::ROOK, board::Color::BLACK));
        double bishop_knight_value = bishop_knight_factor *
                (count_pieces(board::PieceType::ROOK, board::Color::WHITE)
                - count_pieces(board::PieceType::ROOK, board::Color::BLACK)
                + count_pieces(board::PieceType::KNIGHT, board::Color::WHITE)
                - count_pieces(board::PieceType::KNIGHT, board::Color::BLACK));
        double pawn_value = pawn_factor * (count_pawns(board::Color::WHITE) - count_pawns(board::Color::WHITE));
        double dbi_value = double_blocked_isolated_pawn_factor * (WdoubledPawns - BdoubledPawns
                + WblockedPawns - BblockedPawns
                + WisolatedPawns - WisolatedPawns);

        double sign = side == board::Color::WHITE ? 1 : -1;

        return sign * (king_value + queen_value + rook_value +
            bishop_knight_value + pawn_value + dbi_value);
    }

    int Evaluation::count_pieces(PieceType pieceType, Color color)
    {
        auto remainingPieces = chessboard_.getBoardRpr().get(pieceType, color);
        int count = 0;
        while (remainingPieces != 0)
        {
            uint8_t bitToUnset = BitboardOperations::bitScanForward(remainingPieces);
            remainingPieces &= ~(1ul << bitToUnset);
            count++;
        }
        return count;
    }

    int Evaluation::count_pawns(Color color)
    {
        auto remainingPieces = chessboard_.getBoardRpr().get(board::PieceType::PAWN, color);
        auto alliedPawns = chessboard_.getBoardRpr().get(board::PieceType::PAWN, color);
        int count = 0;

        std::array<bool, 8> pawnOnFile = {false};

        while (remainingPieces != 0)
        {
            uint8_t pawnSquare = BitboardOperations::bitScanForward(remainingPieces);
            remainingPieces &= ~(1ul << pawnSquare);

            // bloked pawns
            if (is_pawn_blocked(pawnSquare, color))
                color == board::Color::WHITE ? WblockedPawns++ : BblockedPawns++;
            auto fileNumber = pawnSquare % 8;

            // doubled Pawns
            if (pawnOnFile.at(fileNumber))
                color == board::Color::WHITE ? WdoubledPawns++ : BdoubledPawns++;
            else
                pawnOnFile.at(fileNumber) = true;

            // isolated Pawns
            BitBoard adjFileMask =
                    fileNumber == 0 ?
                        BitboardOperations::arrFileMask[fileNumber + 1]
                    : fileNumber == 7 ?
                        BitboardOperations::arrFileMask[fileNumber - 1]
                    : BitboardOperations::arrFileMask[fileNumber - 1] | BitboardOperations::arrFileMask[fileNumber + 1];

            if (!(alliedPawns & adjFileMask))
                color == board::Color::WHITE ? WisolatedPawns++ : BisolatedPawns++;
            count++;
        }
        return count;
    }

    bool Evaluation::is_pawn_blocked(uint8_t pawnSquare, Color color)
    {
        auto pos = color == board::Color::WHITE ? pawnSquare + 8 : pawnSquare - 8;
        return chessboard_.getBoardRpr().at(Position(pos)).has_value();
    }

}