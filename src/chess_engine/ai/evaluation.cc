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
        auto whiteTurn = chessboard_.isWhiteTurn();
        chessboard.setWhiteTurn(true);
        whiteMoves = chessboard_.generate_legal_moves();
        chessboard.setWhiteTurn(false);
        blackMoves = chessboard_.generate_legal_moves();
        chessboard.setWhiteTurn(whiteTurn);
    }

    float Evaluation::rate_chessboard(Color side)
    {
        int king_factor = 200;
        int queen_factor = 9;
        int rook_factor = 5;
        int bishop_knight_factor = 3;
        int pawn_factor = 1;
        float double_blocked_isolated_pawn_factor = 0.5f;
        float number_moves_factor = 0.1f;

        float king_value = king_factor * (count_pieces(board::PieceType::KING, Color::WHITE)
                                - count_pieces(board::PieceType::KING, board::Color::BLACK));
        float queen_value = queen_factor * (count_pieces(board::PieceType::QUEEN, board::Color::WHITE)
                                - count_pieces(board::PieceType::QUEEN, board::Color::BLACK));
        float rook_value = rook_factor * (count_pieces(board::PieceType::ROOK, board::Color::WHITE)
                                           - count_pieces(board::PieceType::ROOK, board::Color::BLACK));
        float bishop_knight_value = bishop_knight_factor *
                (count_pieces(board::PieceType::ROOK, board::Color::WHITE)
                - count_pieces(board::PieceType::ROOK, board::Color::BLACK)
                + count_pieces(board::PieceType::KNIGHT, board::Color::WHITE)
                - count_pieces(board::PieceType::KNIGHT, board::Color::BLACK));
        float pawn_value = pawn_factor * (count_pawns(board::Color::WHITE) - count_pawns(board::Color::WHITE));
        float dbi_value = double_blocked_isolated_pawn_factor * static_cast<float>(WdoubledPawns - BdoubledPawns
                + WblockedPawns - BblockedPawns
                + WisolatedPawns - WisolatedPawns);
        float moves_value = number_moves_factor * static_cast<float>(whiteMoves.size() - blackMoves.size());

        float sign = side == board::Color::WHITE ? 1 : -1;

        return sign * (king_value + queen_value + rook_value +
            bishop_knight_value + pawn_value + dbi_value + moves_value);
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

        std::array<bool, 7> pawnOnFile = {false};

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
        auto startPos = Position(pawnSquare);
        for (auto move : color == board::Color::WHITE ? whiteMoves : blackMoves)
        {
            if (move.start_pos_get() == startPos)
                return false;
        }
        return true;
    }

}