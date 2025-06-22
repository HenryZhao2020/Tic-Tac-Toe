#pragma once

#include <QList>

enum class SquareIcon;

/**
 * @brief Implements a robust AI for Tic-Tac-Toe using the Minimax algorithm.
 *
 * GameAI evaluates the current board state and determines optimal moves.
 * It can identify winning conditions, draw scenarios, and the best next move.
 *
 * The board is expected to contain 9 SquareIcon representing a 3x3 grid.
 */
class GameAI {
public:
    explicit GameAI();

    SquareIcon getWinner();
    const QList<int> *getWinSeq() const;
    bool isDraw() const;
    bool isRoundEnded();

    int getNextMoveAI();

private:
    static QList<QList<int>> findWinSeqs();

    bool isWinSeq(const QList<int> &seq) const;
    int minimax(int depth, bool maximizeO);

    static const QList<QList<int>> WIN_SEQS;
    static QList<SquareIcon> &board;

    SquareIcon winner;
    const QList<int> *winSeq{nullptr};
    bool roundEnded{false};
};
