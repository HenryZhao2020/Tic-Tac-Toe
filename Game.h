#pragma once

#include <QMainWindow>
#include <QVBoxLayout>

class GameBar;
class Board;
class ScoreBoard;

/**
 * @brief Main window for the Tic-Tac-Toe game.
 *
 * Hosts and manages the core UI components including the game bar,
 * game board, and scoreboard. Handles major game control actions
 * such as restarting, loading saved data, and resuming rounds.
 */
class Game : public QMainWindow {
    Q_OBJECT

public:
    explicit Game();
    ~Game();

    GameBar *getGameBar();
    Board *getBoard();
    ScoreBoard *getScoreBoard();

    void restart();
    void loadSave();
    void resumeRound();

private:
    QVBoxLayout *mainLayout;
    GameBar *gameBar;
    Board *board;
    ScoreBoard *scoreBoard;
};
