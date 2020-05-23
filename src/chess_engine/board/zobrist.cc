#include "zobrist.hh"

#include <random>
uint64_t board::Zobrist::randomArray[2][6][64];
uint64_t board::Zobrist::randomArray2[1 + 4 + 8];

namespace board
{
    void Zobrist::initRandomArray()
    {
        auto randEngine = std::mt19937_64(42);
        for (auto& i : randomArray)
        {
            for (auto& j : i)
            {
                for (unsigned long& k : j)
                {
                    k = randEngine();
                }
            }
        }
        for (auto& i : randomArray2)
        {
            i = randEngine();
        }
    }

    uint64_t Zobrist::hash(board::Chessboard cb)
    {
        uint64_t hash = 0;
        for (int i = 0; i < 6; i++) // for each piece
        {
            for (int j = 0; j < 2; j++) // for each color
            {
                BitBoard remainingPieces = cb.getBoardRpr().get(
                    static_cast<PieceType>(i), static_cast<Color>(j));
                while (remainingPieces) // while there are stil pieces
                {
                    auto squarePiece =
                        BitboardOperations::bitScanForward(remainingPieces);

                    remainingPieces &= ~(1ul << squarePiece);

                    int file = squarePiece % 8;
                    int rank = squarePiece / 8;

                    uint64_t hashValue = randomArray[j][i][rank * 8 + file];

                    if (hash == 0)
                        hash = hashValue;
                    else
                        hash ^= hashValue;
                }
            }
        }
        if (!cb.isWhiteTurn())
        {
            hash ^= randomArray2[0];
        }

        if (!cb.white_king_moved)
        {
            if (!cb.white_queen_rook_moved)
                hash ^= randomArray2[3];
            if (!cb.white_king_rook_moved)
                hash ^= randomArray2[4];
        }
        if (!cb.black_king_moved)
        {
            if (!cb.black_king_rook_moved)
                hash ^= randomArray2[1];
            if (!cb.black_queen_rook_moved)
                hash ^= randomArray2[2];
        }

        auto enPassant = cb.getEnPassant().top();
        if (enPassant)
        {
            auto enPassantFile =
                BitboardOperations::bitScanForward(enPassant) % 8;
            hash ^= randomArray2[5 + enPassantFile];
        }
        return hash;
    }

    uint64_t Zobrist::updateHashWithMove(uint64_t oldHash, Move move,
                                         Chessboard cb)
    {
        auto colorInt = cb.isWhiteTurn() ? 0 : 1;
        auto reverseColorInt = !cb.isWhiteTurn() ? 1 : 0;

        auto pieceTypeInt = static_cast<int>(move.piece_get());
        auto srcrankInt = static_cast<int>(move.start_pos_get().rank_get());
        auto srcfileInt = static_cast<int>(move.start_pos_get().file_get());
        auto dstrankInt = static_cast<int>(move.dest_pos_get().rank_get());
        auto dstfileInt = static_cast<int>(move.dest_pos_get().file_get());

        auto srcNumber =
            randomArray[colorInt][pieceTypeInt][srcrankInt * 8 + srcfileInt];
        auto dstNumber =
            randomArray[colorInt][pieceTypeInt][dstrankInt * 8 + dstfileInt];
        if (move.get_promotion().has_value())
        {
            auto promotionTypeInt =
                static_cast<int>(move.get_promotion().value());
            dstNumber = randomArray[colorInt][promotionTypeInt]
                                   [dstrankInt * 8 + dstfileInt];
        }

        auto newHash = oldHash ^ srcNumber ^ dstNumber;

        if (move.getCapture().has_value())
        {
            auto pieceTypeCaptureInt =
                static_cast<int>(move.getCapture().value());
            auto captureNumber =
                randomArray[reverseColorInt][pieceTypeCaptureInt]
                           [dstrankInt * 8 + dstfileInt];
            newHash ^= captureNumber;
        }
        if (move.isDoublePawnPush())
        {
            newHash ^= randomArray2[5 + dstfileInt];
        }
        if (move.isQueenCastling())
        {
            if (cb.isWhiteTurn())
            {
                newHash ^= randomArray2[3];
            } else
                newHash ^= randomArray2[2];
        } else if (move.isKingCastling())
        {
            if (cb.isWhiteTurn())
            {
                newHash ^= randomArray2[4];
            } else
                newHash ^= randomArray2[1];
        }

        // RESET en passant
        auto enPassant = cb.getEnPassant().top();
        if (enPassant)
        {
            auto enPassantFile =
                BitboardOperations::bitScanForward(enPassant) % 8;
            newHash ^= randomArray2[5 + enPassantFile];
        }
        return newHash ^ randomArray2[0]; // changes sides
    }
} // namespace board
