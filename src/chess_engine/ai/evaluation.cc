#include "evaluation.hh"

#include "../board/bitboard-operations.hh"

namespace ai
{
    Evaluation::Evaluation(Chessboard& chessboard)
        : chessboard_(chessboard)
    {
        WisolatedPawns = 0;
        WblockedPawns = 0;
        WdoubledPawns = 0;
        BisolatedPawns = 0;
        BblockedPawns = 0;
        BdoubledPawns = 0;

        whiteIsCheckmated = false;
        blackIsCheckmated = false;

        chessboard_ = chessboard;
    }

    double Evaluation::rate_chessboard(Color side)
    {
        double checkmate_factor = 20000;
        double queen_factor = 900;
        double rook_factor = 500;
        double knight_factor = 320;
        double bishop_factor = 330;
        double double_blocked_isolated_pawn_factor = 5;

        double checkmate_value =
            checkmate_factor * (blackIsCheckmated - whiteIsCheckmated);
        double king_value =
            count_pieces(PieceType::KING, board::Color::WHITE, 0)
            - count_pieces(PieceType::KING, board::Color::BLACK, 0);
        double queen_value = count_pieces(board::PieceType::QUEEN,
                                          board::Color::WHITE, queen_factor)
            - count_pieces(board::PieceType::QUEEN, board::Color::BLACK,
                           queen_factor);
        double rook_value = count_pieces(board::PieceType::ROOK,
                                         board::Color::WHITE, rook_factor)
            - count_pieces(board::PieceType::ROOK, board::Color::BLACK,
                           rook_factor);
        double knight_value = count_pieces(board::PieceType::KNIGHT,
                                           board::Color::WHITE, knight_factor)
            - count_pieces(board::PieceType::KNIGHT, board::Color::BLACK,
                           knight_factor);
        double bishop_value = count_pieces(board::PieceType::BISHOP,
                                           board::Color::WHITE, bishop_factor)
            - count_pieces(board::PieceType::BISHOP, board::Color::BLACK,
                           bishop_factor);
        double pawn_value =
            count_pawns(board::Color::WHITE) - count_pawns(board::Color::BLACK);
        double dbi_value = double_blocked_isolated_pawn_factor
            * (WdoubledPawns - BdoubledPawns + WblockedPawns - BblockedPawns
               + WisolatedPawns - BisolatedPawns);

        double sign = side == board::Color::WHITE ? +1 : -1;

        return sign
            * (checkmate_value + queen_value + rook_value + bishop_value
               + knight_value + pawn_value + dbi_value + king_value);
    }

    double Evaluation::count_pieces(PieceType pieceType, Color color,
                                    double pieceValue)
    {
        auto remainingPieces = chessboard_.getBoardRpr().get(pieceType, color);
        double count = 0;
        while (remainingPieces != 0)
        {
            uint8_t bitToUnset =
                BitboardOperations::bitScanForward(remainingPieces);
            int rawRank = bitToUnset / 8;
            int file = bitToUnset % 8;
            int rank = color == Color::WHITE ? 7 - rawRank : rawRank;
            int end_game_king =
                pieceType == board::PieceType::KING && is_end_game() ? 1 : 0;

            remainingPieces &= ~(1ul << bitToUnset);

            count += pieceValue
                + tables[static_cast<int>(pieceType) + end_game_king][rank]
                        [file];
        }
        return count;
    }

    double Evaluation::count_pawns(Color color)
    {
        auto remainingPieces =
            chessboard_.getBoardRpr().get(board::PieceType::PAWN, color);
        auto alliedPawns =
            chessboard_.getBoardRpr().get(board::PieceType::PAWN, color);
        double count = 0;

        std::array<bool, 8> pawnOnFile = {false};

        while (remainingPieces != 0)
        {
            uint8_t pawnSquare =
                BitboardOperations::bitScanForward(remainingPieces);
            remainingPieces &= ~(1ul << pawnSquare);

            // bloked pawns
            if (is_pawn_blocked(pawnSquare, color))
                color == board::Color::WHITE ? WblockedPawns++
                                             : BblockedPawns++;
            auto fileNumber = pawnSquare % 8;

            // doubled Pawns
            if (pawnOnFile.at(fileNumber))
                color == board::Color::WHITE ? WdoubledPawns++
                                             : BdoubledPawns++;
            else
                pawnOnFile.at(fileNumber) = true;

            // isolated Pawns
            BitBoard adjFileMask = fileNumber == 0
                ? BitboardOperations::arrFileMask[fileNumber + 1]
                : fileNumber == 7
                    ? BitboardOperations::arrFileMask[fileNumber - 1]
                    : BitboardOperations::arrFileMask[fileNumber - 1]
                        | BitboardOperations::arrFileMask[fileNumber + 1];

            if (!(alliedPawns & adjFileMask))
                color == board::Color::WHITE ? WisolatedPawns++
                                             : BisolatedPawns++;

            int rawRank = pawnSquare / 8;
            int file = pawnSquare % 8;
            int rank = color == Color::WHITE ? 7 - rawRank : rawRank;
            count +=
                100 + tables[static_cast<int>(PieceType::PAWN)][rank][file];

            count++;
        }
        return count;
    }

    bool Evaluation::is_pawn_blocked(uint8_t pawnSquare, Color color)
    {
        auto pos =
            color == board::Color::WHITE ? pawnSquare + 8 : pawnSquare - 8;
        return chessboard_.getBoardRpr().at(Position(pos)).has_value();
    }

    bool Evaluation::is_end_game() // we're in the end game now.
    {
        auto& rpr = chessboard_.getBoardRpr();
        if (!rpr.get(board::PieceType::QUEEN, board::Color::WHITE)
            && !rpr.get(board::PieceType::QUEEN, board::Color::BLACK))
        {
            return true;
        } else if (rpr.get(board::PieceType::QUEEN, board::Color::WHITE)
                   && rpr.get(board::PieceType::QUEEN, board::Color::BLACK))
        {
            return check_has_qmp(rpr, board::Color::WHITE)
                && check_has_qmp(rpr, board::Color::BLACK);
        } else if (rpr.get(board::PieceType::QUEEN, board::Color::WHITE))
        {
            return check_has_qmp(rpr, board::Color::WHITE);
        } else if (rpr.get(board::PieceType::QUEEN, board::Color::BLACK))
        {
            return check_has_qmp(rpr, board::Color::BLACK);
        }
        return false;
    }

    bool Evaluation::check_has_qmp(Chessboard_rpr& rpr, Color color) const
    {
        auto wp_knight = rpr.get(PieceType::KNIGHT, color);
        auto wp_bishop = rpr.get(PieceType::BISHOP, color);
        auto wp_rook = rpr.get(PieceType::ROOK, color);
        auto toUnsetBishop = BitboardOperations::bitScanForward(wp_bishop);
        auto toUnsetKnight = BitboardOperations::bitScanForward(wp_knight);
        wp_knight &= ~(1UL << toUnsetKnight);
        wp_bishop &= ~(1UL << toUnsetBishop);
        return !wp_rook
            && ((toUnsetBishop == -1 && !wp_knight)
                || (!wp_bishop && toUnsetKnight == -1));
    }
} // namespace ai