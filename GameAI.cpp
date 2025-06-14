#include "GameAI.h"
#include "Attr.h"

QList<SquareIcon> &GameAI::board{Attr::getProgress().board};
const QList<QList<int>> GameAI::WIN_SEQS{findWinSeqs()};

GameAI::GameAI() : winner{SquareIcon::NONE} {}

GameAI::~GameAI() {}

SquareIcon GameAI::getWinner() {
    if (roundEnded) return winner;

    for (const auto &seq : WIN_SEQS) {
        if (isWinSeq(seq) && board[seq[0]] != SquareIcon::NONE) {
            winSeq = &seq;
            return board[seq[0]];
        }
    }
    return SquareIcon::NONE;
}

const QList<int> *GameAI::getWinSeq() const {
    return winSeq;
}

bool GameAI::isDraw() const {
    return roundEnded ? (winner == SquareIcon::NONE)
                      : !board.contains(SquareIcon::NONE);
}

bool GameAI::isRoundEnded() {
    if (!roundEnded) {
        roundEnded = isDraw() || (winner = getWinner()) != SquareIcon::NONE;
    }
    return roundEnded;
}

int GameAI::getNextMoveAI() {
    int bestMove{-1};           // Store the index of the best move found
    int bestScore{INT_MIN};     // Initialize best score to lowest possible

    for (int i = 0; i < BOARD_DIM * BOARD_DIM; ++i) {
        if (board[i] == SquareIcon::NONE) {         // Try only empty squares
            board[i] = SquareIcon::O;               // Simulate AI's move (O)
            const int score{minimax(0, false)};     // Evaluate move using minimax
            board[i] = SquareIcon::NONE;            // Undo move (backtrack)

            if (score > bestScore) {                // Update best score and move
                bestScore = score;
                bestMove = i;
            }
        }
    }

    winSeq = nullptr;  // Clear any temporary winning sequence in GameAI::getWinner()
    return bestMove;
}

QList<QList<int>> GameAI::findWinSeqs() {
    QList<QList<int>> winSeqs;
    QList<int> leftDiag, rightDiag;

    for (int i = 0; i < BOARD_DIM; ++i) {
        QList<int> row, col;
        for (int j = 0; j < BOARD_DIM; ++j) {
            row.append(i * BOARD_DIM + j);
            col.append(j * BOARD_DIM + i);
        }
        winSeqs.append(row);
        winSeqs.append(col);
        leftDiag.append(i * BOARD_DIM + i);
        rightDiag.append(i * BOARD_DIM + (BOARD_DIM - i - 1));
    }

    winSeqs.append(leftDiag);
    winSeqs.append(rightDiag);
    return winSeqs;
}

bool GameAI::isWinSeq(const QList<int> &seq) const {
    for (int i = 1; i < BOARD_DIM; ++i) {
        if (board[seq[i - 1]] != board[seq[i]]) {
            return false;
        }
    }
    return true;
}

int GameAI::minimax(int depth, bool maximizeO) {
    // Terminal state: return static evaluation of the board
    const SquareIcon winner{getWinner()};
    if (winner == SquareIcon::O) return 1;     // AI wins
    if (winner == SquareIcon::X) return -1;    // Opponent wins
    if (isDraw()) return 0;                    // Game is a draw

    // AI's turn: maximize the score
    if (maximizeO) {
        int bestScore{INT_MIN};
        for (int i = 0; i < BOARD_DIM * BOARD_DIM; ++i) {
            if (board[i] == SquareIcon::NONE) {
                board[i] = SquareIcon::O;                       // Simulate AI move
                const int score{minimax(depth + 1, false)};     // Recurse as opponent
                board[i] = SquareIcon::NONE;                    // Undo move
                bestScore = std::max(score, bestScore);         // Track best score
            }
        }
        return bestScore;
    }

    // Opponent's turn: minimize the score
    else {
        int bestScore{INT_MAX};
        for (int i = 0; i < BOARD_DIM * BOARD_DIM; ++i) {
            if (board[i] == SquareIcon::NONE) {
                board[i] = SquareIcon::X;                       // Simulate opponent move
                const int score{minimax(depth + 1, true)};      // Recurse as AI
                board[i] = SquareIcon::NONE;
                bestScore = std::min(score, bestScore);         // Track worst-case outcome
            }
        }
        return bestScore;
    }
}
