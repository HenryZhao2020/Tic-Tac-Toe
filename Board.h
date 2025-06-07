#pragma once

#include <QFrame>

class Game;
class GameBar;
class Square;
class GameAI;

enum class SquareIcon;

/**
 * @brief Displays a 3x3 Tic-Tac-Toe board and handles user interaction.
 *
 * Manages the visual layout and state of the game board, including rendering
 * square icons (X, O, or empty) and processing player input.
 * Integrates with the main game logic to update board state and notify
 * other components of player moves.
 */
class Board : public QFrame {
    Q_OBJECT

public:
    explicit Board(Game *game);
    ~Board();

    void place(int i, const QIcon &icon, bool animated);
    void placeX(int i);
    void placeO(int i = -1);
    void setFrozen(bool frozen);
    bool isFrozen();

private:
    bool isRoundEnded();
    void endRound();

    static GameAI ai;

    Game *game;
    GameBar *gameBar;
    Square *squares[9];
    SquareIcon winner;
    bool frozen;
};
