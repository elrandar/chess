#pragma once

#include "../board/chessboard.hh"
namespace ai
{
    using namespace board;
    class Evaluation {
    public:
        static constexpr double factorArray[6] = {900, 500, 330, 320, 100, 20000};

        constexpr static std::pair<int, int> doubledFactor = std::pair<int,int>(10,20);
        constexpr static std::pair<int, int> isolatedFactor = std::pair<int,int>(10,20);
        constexpr static std::pair<int, int> blockedFactor = std::pair<int,int>(-40,-60);

        static constexpr double openWhiteTables[6][8][8]{

            {//queen

                    {0, 0, 0, 0, 0, 0 ,0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {-5, -5, -5, -5, -5, -5, -5, -5}
            },
            {//rooks
                    {-6, -3, 0, 3, 3, 0, -3, -6},
                    {-6, -3, 0, 3, 3, 0, -3, -6},
                    {-6, -3, 0, 3, 3, 0, -3, -6},
                    {-6, -3, 0, 3, 3, 0, -3, -6},
                    {-6, -3, 0, 3, 3, 0, -3, -6},
                    {-6, -3, 0, 3, 3, 0, -3, -6},
                    {-6, -3, 0, 3, 3, 0, -3, -6},
                    {-6, -3, 0, 3, 3, 0, -3, -6}
            },
            {//bishop
                    {-8, -8, -6, -4, -4, -6, -8, -8},
                    {-8,  0,  -2,  0,  0,  -2,  0,-8},
                    {-6,  -2,  4, 2, 2,  4,  -2, -6},
                    {-4,  0,  2, 8, 8,  2,  0,-4},
                    {-4,  0,  2, 8, 8,  2,  0,-4},
                    {-6, -2, 4, 2, 2, 4, -2,-6},
                    {-8,  0,  -2,  0,  0,  -2,  0,-8},
                    {-18,-18,-16,-14,-14,-16,-18,-18}
            },
            {//knights
                    {-135, -25, -15, -10, -10, -15, -25, -135},
                    {-20, -10, 0, 5, 5, 0, -10, -20},
                    {-5, 5, 15, 20, 20, 15, 5, -5},
                    {-5, 5, 15, 20, 20, 15, 5, -5},
                    {-10, 0, 10, 15, 15, 10, 0, -10},
                    {-20, -10, 0, 5, 5, 0, -10, -20},
                    {-35, -25, -15, -10, -10, -15, -25, -35},
                    {-50, -40, -30, -25, -25, -30, -40, -50}
            },
            {//pawn
                {-15,  -5,  0,   5,   5,   0,   -5,  -15},
                {-15,  -5,  0,   5,   5,   0,   -5,  -15},
                {-15,  -5,  0,   5,   5,   0,   -5,  -15},
                {-15,  -5,  0,   15,   15,   0,   -5,  -15},
                {-15,  -5,  0,   25,   25,   0,   -5,  -15},
                {-15,  -5,  0,   15,   15,   0,   -5,  -15},
                {-15,  -5,  0,   5,   5,   0,   -5,  -15},
                {-15,  -5,  0,   5,   5,   0,   -5,  -15},
            },
            {//king
                {-40,-30,-50,-70,-70,-50,-30,-40},
                {-30,-20,-40,-60,-60,-40,-20,-30},
                {-20,-10,-30,-50,-50,-30,-10,-20},
                {-10,0,-20,-40,-40,-20, 0,-10},
                {0, 10,-10,-30,-30,-10, 10, 0},
                {10,20, 0,-20,-20, 0, 20, 10},
                {30, 40,  20,  0,  0,  20, 40, 30},
                {40, 50, 30,  10,  10, 30, 50, 40}
            }
        };

        static constexpr double endWhiteTables[6][8][8]{

            {//queen

                    {-24, -16, -12, -8, -8, -12 ,-16, -24},
                    {-16, -8, -4, 0, 0, -4, -8, -16},
                    {-12, -4, 0, 4, 4, 0 -4, -12},
                    {-8, 0, 4, 8, 8, 4, 0 -8},
                    {-8, 0, 4, 8, 8, 4, 0 -8},
                    {-12, -4, 0, 4, 4, 0 -4, -12},
                    {-16, -8, -4, 0, 0, -4, -8, -16},
                    {-24, -16, -12, -8, -8, -12 ,-16, -24}
            },
            {//rooks
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}
            },
            {//bishop
                    {-18, -12, -9, -6, -6, -9, -12, -18},
                    {-12,  -6,  -3,  0,  0,  -3,  -6,-12},
                    {-9,  -3,  0, 3, 3,  0,  -3, -9},
                    {-6,  0,  3, 6, 6,  3,  0,-6},
                    {-6,  0,  3, 6, 6,  3,  0,-6},
                    {-9,  -3,  0, 3, 3,  0,  -3, -9},
                    {-12,  -6,  -3,  0,  0,  -3,  -6,-12},
                    {-18, -12, -9, -6, -6, -9, -12, -18},
            },
            {//knights
                    {-40, -30, -20, -15, -15, -20, -30, -40},
                    {-30, -20, -10, -5, -5, -10, -20, -30},
                    {-20, -10, 0, 5, 5, 0, -10, -20},
                    {-15, -5, 5, 10, 10, 5, -5, -15},
                    {-15, -5, 5, 10, 10, 5, -5, -15},
                    {-20, -10, 0, 5, 5, 0, -10, -20},
                    {-30, -20, -10, -5, -5, -10, -20, -30},
                    {-40, -30, -20, -15, -15, -20, -30, -40},
            },
            {//pawn
                    {0,  0,  0,   0,   0,   0,   0,  0},
                    {0,  0,  0,   0,   0,   0,   0,  0},
                    {0,  0,  0,   0,   0,   0,   0,  0},
                    {0,  0,  0,   0,   0,   0,   0,  0},
                    {0,  0,  0,   0,   0,   0,   0,  0},
                    {0,  0,  0,   0,   0,   0,   0,  0},
                    {0,  0,  0,   0,   0,   0,   0,  0},
                    {0,  0,  0,   0,   0,   0,   0,  0},
            },
            {//king
                    {-72, -48,-36,-24,-24,-36,-48,-72},
                    {-48,-24,-12, 0, 0,-12,-24,-48},
                    {-36,-12, 0, 12, 12, 0,-12,-36},
                    {-24,0,12,24,24,12, 0,-24},
                    {-24,0,12,24,24,12, 0,-24},
                    {-36,-12, 0, 12, 12, 0,-12,-36},
                    {-48,-24,-12, 0, 0,-12,-24,-48},
                    {-72, -48,-36,-24,-24,-36,-48,-72},
            }
        };


        static constexpr double openBlackTables[6][8][8]{

                {//queen

                        {-5, -5, -5, -5, -5, -5, -5, -5},
                        {0, 0, 0, 0, 0, 0 ,0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                },
                {//rooks
                        {-6, -3, 0, 3, 3, 0, -3, -6},
                        {-6, -3, 0, 3, 3, 0, -3, -6},
                        {-6, -3, 0, 3, 3, 0, -3, -6},
                        {-6, -3, 0, 3, 3, 0, -3, -6},
                        {-6, -3, 0, 3, 3, 0, -3, -6},
                        {-6, -3, 0, 3, 3, 0, -3, -6},
                        {-6, -3, 0, 3, 3, 0, -3, -6},
                        {-6, -3, 0, 3, 3, 0, -3, -6}
                },
                {//bishop
                        {-18,-18,-16,-14,-14,-16,-18,-18},
                        {-8,  0,  -2,  0,  0,  -2,  0,-8},
                        {-6,  -2,  4,  2,  2,   4, -2,-6},
                        {-4,  0,  2,  8,  8,  2,  0, -4},
                        {-4,  0,  2,  8,  8,  2,  0, -4},
                        {-6, -2,  4,  2,  2,  4, -2, -6},
                        {-8,  0, -2,  0,  0,  -2,  0,-8},
                        {-8, -8, -6, -4, -4, -6, -8, -8},
                },
                {//knights
                        {-50, -40, -30, -25, -25, -30, -40, -50},
                        {-35, -25, -15, -10, -10, -15, -25, -35},
                        {-20, -10, 0, 5, 5, 0, -10, -20},
                        {-10, 0, 10, 15, 15, 10, 0, -10},
                        {-5, 5, 15, 20, 20, 15, 5, -5},
                        {-5, 5, 15, 20, 20, 15, 5, -5},
                        {-20, -10, 0, 5, 5, 0, -10, -20},
                        {-135, -25, -15, -10, -10, -15, -25, -135}
                },
                {//pawn
                        {-15,  -5,  0,   5,   5,   0,   -5,  -15},
                        {-15,  -5,  0,   5,   5,   0,   -5,  -15},
                        {-15,  -5,  0,   15,   15,   0,   -5,  -15},
                        {-15,  -5,  0,   25,   25,   0,   -5,  -15},
                        {-15,  -5,  0,   15,   15,   0,   -5,  -15},
                        {-15,  -5,  0,   5,   5,   0,   -5,  -15},
                        {-15,  -5,  0,   5,   5,   0,   -5,  -15},
                        {-15,  -5,  0,   5,   5,   0,   -5,  -15},
                },
                {//king
                        {40, 50, 30,  10,  10, 30, 50, 40},
                        {30, 40,  20,  0,  0,  20, 40, 30},
                        {10,20, 0,-20,-20, 0, 20, 10},
                        {0, 10,-10,-30,-30,-10, 10, 0},
                        {-10,0,-20,-40,-40,-20, 0,-10},
                        {-20,-10,-30,-50,-50,-30,-10,-20},
                        {-30,-20,-40,-60,-60,-40,-20,-30},
                        {-40,-30,-50,-70,-70,-50,-30,-40}
                }
        };

        static constexpr double endBlackTables[6][8][8]{

                {//queen

                        {-24, -16, -12, -8, -8, -12 ,-16, -24},
                        {-16, -8, -4, 0, 0, -4, -8, -16},
                        {-12, -4, 0, 4, 4, 0 -4, -12},
                        {-8, 0, 4, 8, 8, 4, 0 -8},
                        {-8, 0, 4, 8, 8, 4, 0 -8},
                        {-12, -4, 0, 4, 4, 0 -4, -12},
                        {-16, -8, -4, 0, 0, -4, -8, -16},
                        {-24, -16, -12, -8, -8, -12 ,-16, -24}
                },
                {//rooks
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0}
                },
                {//bishop
                        {-18, -12, -9, -6, -6, -9, -12, -18},
                        {-12,  -6,  -3,  0,  0,  -3,  -6,-12},
                        {-9,  -3,  0, 3, 3,  0,  -3, -9},
                        {-6,  0,  3, 6, 6,  3,  0,-6},
                        {-6,  0,  3, 6, 6,  3,  0,-6},
                        {-9,  -3,  0, 3, 3,  0,  -3, -9},
                        {-12,  -6,  -3,  0,  0,  -3,  -6,-12},
                        {-18, -12, -9, -6, -6, -9, -12, -18},
                },
                {//knights
                        {-40, -30, -20, -15, -15, -20, -30, -40},
                        {-30, -20, -10, -5, -5, -10, -20, -30},
                        {-20, -10, 0, 5, 5, 0, -10, -20},
                        {-15, -5, 5, 10, 10, 5, -5, -15},
                        {-15, -5, 5, 10, 10, 5, -5, -15},
                        {-20, -10, 0, 5, 5, 0, -10, -20},
                        {-30, -20, -10, -5, -5, -10, -20, -30},
                        {-40, -30, -20, -15, -15, -20, -30, -40},
                },
                {//pawn
                        {0,  0,  0,   0,   0,   0,   0,  0},
                        {0,  0,  0,   0,   0,   0,   0,  0},
                        {0,  0,  0,   0,   0,   0,   0,  0},
                        {0,  0,  0,   0,   0,   0,   0,  0},
                        {0,  0,  0,   0,   0,   0,   0,  0},
                        {0,  0,  0,   0,   0,   0,   0,  0},
                        {0,  0,  0,   0,   0,   0,   0,  0},
                        {0,  0,  0,   0,   0,   0,   0,  0},
                },
                {//king
                        {-72, -48,-36,-24,-24,-36,-48,-72},
                        {-48,-24,-12, 0, 0,-12,-24,-48},
                        {-36,-12, 0, 12, 12, 0,-12,-36},
                        {-24,0,12,24,24,12, 0,-24},
                        {-24,0,12,24,24,12, 0,-24},
                        {-36,-12, 0, 12, 12, 0,-12,-36},
                        {-48,-24,-12, 0, 0,-12,-24,-48},
                        {-72, -48,-36,-24,-24,-36,-48,-72},
                }
        };

        Chessboard &chessboard_;

        int WdoubledPawns;
        int WblockedPawns;
        int WisolatedPawns;
        int BdoubledPawns;
        int BblockedPawns;
        int BisolatedPawns;

        bool whiteIsCheckmated;
        bool blackIsCheckmated;

        explicit Evaluation(Chessboard &chessboard);
        double rate_chessboard(board::Color side);
        double count_pieces_and_pos(PieceType pieceType, Color color);
        double count_pawns(Color color);
        bool is_pawn_blocked(uint8_t pawnSquare, Color color);
        double white_opponent_checkmated();
        double black_opponent_checkmated();
        double eval_white();
        double eval_black();

        double pawns_evaluation(Color color);
    };
}



