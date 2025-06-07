#pragma once

#include <QList>

enum class SquareIcon;

/**
 * @brief Implements a simple AI for Tic-Tac-Toe using the Minimax algorithm.
 *
 * GameAI evaluates the current board state and determines optimal moves.
 * It can identify winning conditions, draw scenarios, and the best next move.
 *
 * The board is passed by reference and expected to have 9 elements representing a 3x3 grid.
 */
class GameAI {
public:
    explicit GameAI(QList<SquareIcon> &board);
    ~GameAI();

    SquareIcon getWinner();
    QList<int> getWinSeq();
    bool isDraw();
    int getBestMove();

private:
    int minimax(int depth, bool isMaximizing);

    static const QList<QList<int>> WIN_SEQS;
    QList<SquareIcon> &board;
};
