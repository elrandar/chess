#include "evaluation.hh"

#include "../board/bitboard-operations.hh"
#include "../board/magic.hh"
#include "../board/masks.hh"
#include "tools.hh"

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

        gamePhase = tools::gamePhase(chessboard.getBoardRpr());
    }

    double Evaluation::rate_chessboard(Color side)
    {
        auto white_eval = eval_white();
        auto black_eval = eval_black();
        return side == board::Color::WHITE ? white_eval + (-black_eval)
                                           : black_eval + (-white_eval);
    }

    double Evaluation::count_pieces_and_pos(PieceType pieceType, Color color)
    {
        auto remainingPieces = chessboard_.getBoardRpr().get(pieceType, color);
        double count = 0;
        while (remainingPieces != 0)
        {
            uint8_t bitToUnset =
                BitboardOperations::bitScanForward(remainingPieces);
            int rawRank = bitToUnset / 8;
            int file = bitToUnset % 8;
            int rank = 7 - rawRank;

            remainingPieces &= ~(1ul << bitToUnset);

            auto opening = color == board::Color::WHITE
                ? openWhiteTables[static_cast<int>(pieceType)][rank][file]
                : openBlackTables[static_cast<int>(pieceType)][rank][file];
            auto endgame = color == board::Color::WHITE
                ? endWhiteTables[static_cast<int>(pieceType)][rank][file]
                : endBlackTables[static_cast<int>(pieceType)][rank][file];
            auto factor = pieceType == board::PieceType::KING
                ? 0
                : factorArray[static_cast<int>(pieceType)];

            auto openvalue = ((float)(24 - gamePhase) / (float)24) * opening;
            auto endvalue = (gamePhase / (float)24) * endgame;

            count += factor + openvalue + endvalue;
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
            int rank = 7 - rawRank;

            auto opening = color == board::Color::WHITE
                ? openWhiteTables[static_cast<int>(PieceType::PAWN)][rank][file]
                : openBlackTables[static_cast<int>(PieceType::PAWN)][rank]
                                 [file];
            auto endgame = color == board::Color::WHITE
                ? endWhiteTables[static_cast<int>(PieceType::PAWN)][rank][file]
                : endBlackTables[static_cast<int>(PieceType::PAWN)][rank][file];

            auto openvalue = ((float)(24 - gamePhase) / (float)24) * opening;
            auto endvalue = (gamePhase / (float)24) * endgame;

            count += factorArray[static_cast<int>(PieceType::PAWN)] + openvalue
                + endvalue;

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

    double Evaluation::pawns_evaluation(Color color)
    {
        double eval = 0;
        eval = count_pawns(color);

        auto isolated =
            color == board::Color::WHITE ? WisolatedPawns : BisolatedPawns;
        auto blocked =
            color == board::Color::WHITE ? WblockedPawns : BblockedPawns;
        auto doubled =
            color == board::Color::WHITE ? WdoubledPawns : BdoubledPawns;
        auto openPhase = (24 - gamePhase) / 24;
        auto endPhase = gamePhase / 24;

        eval += (isolatedFactor.first * openPhase
                 + isolatedFactor.second * endPhase)
            * isolated;
        eval +=
            (blockedFactor.first * openPhase + blockedFactor.second * endPhase)
            * blocked;
        eval +=
            (doubledFactor.first * openPhase + doubledFactor.second * endPhase)
            * doubled;

        eval += pawn_king_defense(color);

        return eval;
    }

    double Evaluation::eval_white()
    {
        double eval = 0;
        for (int i = 0; i < 6; i++)
        {
            if (i == static_cast<int>(PieceType::PAWN))
                eval += pawns_evaluation(board::Color::WHITE);
            else
                eval += count_pieces_and_pos(static_cast<PieceType>(i),
                                             Color::WHITE);
            if (i == static_cast<int>(PieceType::KING))
            {
                eval += white_opponent_checkmated();
            }
        }
        eval += piece_attack(Color::WHITE);
        return eval;
    }
    double Evaluation::eval_black()
    {
        double eval = 0;
        for (int i = 0; i < 6; i++)
        {
            if (i == 4)
                eval += pawns_evaluation(board::Color::BLACK);
            else
                eval += count_pieces_and_pos(static_cast<PieceType>(i),
                                             Color::BLACK);
            if (i == static_cast<int>(PieceType::KING))
            {
                eval += black_opponent_checkmated();
            }
        }
        eval += piece_attack(Color::BLACK);
        return eval;
    }

    double Evaluation::white_opponent_checkmated()
    {
        return factorArray[static_cast<int>(PieceType::KING)]
            * blackIsCheckmated;
    }

    double Evaluation::black_opponent_checkmated()
    {
        return factorArray[static_cast<int>(PieceType::KING)]
            * whiteIsCheckmated;
    }

    double Evaluation::pawn_king_defense(Color color)
    {
        auto& rpr = chessboard_.getBoardRpr();
        auto opponent_color = color == board::Color::WHITE
            ? board::Color::BLACK
            : board::Color::WHITE;
        if (rpr.get(board::PieceType::QUEEN, opponent_color)
            && (rpr.get(board::PieceType::ROOK, opponent_color)
                | rpr.get(board::PieceType::BISHOP, opponent_color)
                | rpr.get(board::PieceType::KNIGHT, opponent_color)))
        {
            auto eval = 0;
            eval += pawn_shelter(color);
            eval += pawn_storm_castling(color);
            return eval * (static_cast<float>(24 - gamePhase))
                / (static_cast<float>(24));
            ;
        } else
            return 0;
    }

    double Evaluation::pawn_shelter(Color color)
    {
        auto eval = 0;
        auto& rpr = chessboard_.getBoardRpr();
        auto kingBoard = rpr.get(board::PieceType::KING, color);
        auto kingFile = BitboardOperations::bitScanForward(kingBoard) % 8;

        if (kingFile != 0)
        {
            eval += tools::fpap(
                color, rpr,
                BitboardOperations::arrFileMask[static_cast<int>(kingFile)
                                                + 1]);
        }
        if (kingFile != 7)
        {
            eval += tools::fpap(
                color, rpr,
                BitboardOperations::arrFileMask[static_cast<int>(kingFile)
                                                - 1]);
        }

        eval += 2
            * (tools::fpap(
                color, rpr,
                BitboardOperations::arrFileMask[static_cast<int>(kingFile)]));

        if (eval == 0)
            return -11;
        else
            return eval;
    }

    double Evaluation::pawn_storm(Color color, BitBoard fileToCheck)
    {
        auto eval = 0;
        auto& rpr = chessboard_.getBoardRpr();

        if (fileToCheck != 0)
        {
            eval += tools::hpap(color, rpr, fileToCheck + 1);
        }
        if (fileToCheck != 7)
        {
            eval += tools::hpap(color, rpr, fileToCheck - 1);
        }

        eval += tools::hpap(color, rpr, fileToCheck);

        return eval;
    }

    double Evaluation::pawn_storm_castling(Color color)
    {
        auto king_moved = color == board::Color::WHITE
            ? chessboard_.white_king_moved
            : chessboard_.black_king_moved;
        auto queen_rook_moved = color == board::Color::WHITE
            ? chessboard_.white_queen_rook_moved
            : chessboard_.black_queen_rook_moved;
        auto king_rook_moved = color == board::Color::WHITE
            ? chessboard_.white_king_rook_moved
            : chessboard_.black_king_moved;

        auto& rpr = chessboard_.getBoardRpr();
        auto kingBoard = rpr.get(board::PieceType::KING, color);
        auto kingIndex = BitboardOperations::bitScanForward(kingBoard);
        auto kingFile = BitboardOperations::arrFileMask[kingIndex % 8];

        auto king_pawn_storm = pawn_storm(color, kingFile);

        if (!king_moved && (!queen_rook_moved || !king_rook_moved))
        {
            auto castling_pawn_storm = 0;
            if (!queen_rook_moved)
            {
                castling_pawn_storm =
                    pawn_storm(color, BitboardOperations::arrFileMask[0]);
            }
            if (!king_rook_moved)
            {
                auto tmp_pawn_storm_castling =
                    pawn_storm(color, BitboardOperations::arrFileMask[7]);
                if (tmp_pawn_storm_castling > castling_pawn_storm)
                    castling_pawn_storm = tmp_pawn_storm_castling;
            }
            return (king_pawn_storm + castling_pawn_storm) / 2;
        } else
            return king_pawn_storm;
    }

    double Evaluation::piece_attack(Color color)
    {
        auto& rpr = chessboard_.getBoardRpr();

        Color enemyColor =
            color == board::Color::WHITE ? Color::BLACK : Color::WHITE;

        auto kingBoard = rpr.get(board::PieceType::KING, enemyColor);
        if (!kingBoard)
            return 0;

        auto kingIndex = BitboardOperations::bitScanForward(kingBoard);

        auto knights = rpr.get(board::PieceType::KNIGHT, color);

        int numberOfKnights =
            numberOfKnightsAttackingAdjacentSquares(kingIndex, knights);

        // build mask used for bishop rook and queen.

        auto kingMaskOneMove = board::Masks::king_attacks(kingIndex);
        //    * * *
        //    * K *
        //    * * *

        auto remainingSquares = kingMaskOneMove;

        auto rooks = rpr.get(board::PieceType::ROOK, color);
        auto bishops = rpr.get(board::PieceType::BISHOP, color);
        auto queens = rpr.get(board::PieceType::QUEEN, color);

        BitBoard attackingRooks = 0;
        BitBoard attackingBishops = 0;
        BitBoard attackingQueens = 0;

        while (remainingSquares)
        {
            auto sqToAttack =
                BitboardOperations::bitScanForward(remainingSquares);
            remainingSquares &= ~(1ul << sqToAttack);

            auto rookDirectAttacks =
                Masks::rook_attack_rays[Masks::NORTH][sqToAttack]
                | Masks::rook_attack_rays[Masks::EAST][sqToAttack]
                | Masks::rook_attack_rays[Masks::WEST][sqToAttack]
                | Masks::rook_attack_rays[Masks::SOUTH][sqToAttack];
            auto bishopDirectAttacks =
                Masks::bishop_attack_rays[Masks::NORTH_EAST][sqToAttack]
                | Masks::bishop_attack_rays[Masks::SOUTH_EAST][sqToAttack]
                | Masks::bishop_attack_rays[Masks::NORTH_WEST][sqToAttack]
                | Masks::bishop_attack_rays[Masks::SOUTH_WEST][sqToAttack];

            attackingRooks |= rookDirectAttacks & rooks;
            attackingBishops |= bishopDirectAttacks & bishops;
            attackingQueens |=
                (rookDirectAttacks | bishopDirectAttacks) & queens;
        }

        auto kingMaskTwoMoves = kingMaskOneMove;
        //  * * * * *
        //  * * * * *
        //  * * K * *
        //  * * * * *
        //  * * * * *

        remainingSquares = kingMaskOneMove;

        while (remainingSquares)
        {
            auto attackSquare =
                BitboardOperations::bitScanForward(remainingSquares);
            remainingSquares &= ~(1ul << attackSquare);

            kingMaskTwoMoves |= board::Masks::king_attacks(attackSquare);
        }

        BitBoard attackingTwoQueens = 0;
        BitBoard attackingTwoRooks = 0;
        BitBoard attackingTwoBishops = 0;

        while (kingMaskTwoMoves)
        {
            auto attackSquare =
                BitboardOperations::bitScanForward(kingMaskTwoMoves);
            kingMaskTwoMoves &= ~(1ul << attackSquare);

            auto bishopRealAttack =
                board::magic::attack_bishop(rpr.occupied, attackSquare);
            auto rookRealAttack =
                board::magic::attack_rook(rpr.occupied, attackSquare);

            attackingTwoQueens |= (bishopRealAttack | rookRealAttack) & queens;
            attackingTwoBishops |= bishopRealAttack & bishops;
            attackingTwoRooks |= rookRealAttack & rooks;
        }

        attackingBishops &= attackingTwoBishops;
        attackingQueens &= attackingTwoQueens;
        attackingRooks &= attackingTwoRooks;

        int nbAttackBishop = __builtin_popcount(attackingBishops);
        int nbAttackRook = __builtin_popcount(attackingRooks);
        int nbAttackQueen = __builtin_popcount(attackingQueens);

        int valuedSum = nbAttackBishop + nbAttackQueen * 4 + nbAttackRook * 2
            + numberOfKnights;

        int numberOfAttackers =
            nbAttackQueen + nbAttackRook + nbAttackBishop + numberOfKnights;
        if (numberOfAttackers > 7)
            numberOfAttackers = 7;
        return 20 * valuedSum * pieceAttackWeights[numberOfAttackers - 1];
    }

    int Evaluation::numberOfKnightsAttackingAdjacentSquares(int kingIndex,
                                                            BitBoard knights)
    {
        int totalKnightsAttacking = 0;

        if (kingIndex % 8 != 7)
        {
            auto knightsAttackingRightSquare =
                Masks::knight_attacks(kingIndex + 1) & knights;
            totalKnightsAttacking +=
                __builtin_popcount(knightsAttackingRightSquare);
        }
        if (kingIndex % 8 != 0)
        {
            auto knightsAttackingLeftSquare =
                Masks::knight_attacks(kingIndex - 1) & knights;
            totalKnightsAttacking +=
                __builtin_popcount(knightsAttackingLeftSquare);
        }
        if (kingIndex > 7)
        {
            auto knightsAttackingBottomSquare =
                Masks::knight_attacks(kingIndex - 8) & knights;
            totalKnightsAttacking +=
                __builtin_popcount(knightsAttackingBottomSquare);
        }
        if (kingIndex < 56)
        {
            auto knightsAttackingTopSquare =
                Masks::knight_attacks(kingIndex + 8) & knights;
            totalKnightsAttacking +=
                __builtin_popcount(knightsAttackingTopSquare);
        }
        return totalKnightsAttacking;
    }
} // namespace ai