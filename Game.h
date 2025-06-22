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
 * game board, and scoreboard.
 */
class Game : public QMainWindow {
    Q_OBJECT

public:
    explicit Game();

    GameBar *getGameBar() const;
    Board *getBoard() const;
    ScoreBoard *getScoreBoard() const;

    void restart();
    void loadSave();
    void resumeRound();

    void show();

private:
    QVBoxLayout *mainLayout{nullptr};
    GameBar *gameBar{nullptr};
    Board *board{nullptr};
    ScoreBoard *scoreBoard{nullptr};
};
