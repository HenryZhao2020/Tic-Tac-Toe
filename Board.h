#pragma once

#include <QFrame>

class Game;
class GameBar;
class Square;
class GameAI;
enum class SquareIcon;

/**
 * @brief Displays a 3x3 Tic-Tac-Toe board and handles user interaction.
 */
class Board : public QFrame {
    Q_OBJECT

public:
    static constexpr int TURN_DELAY{500};

    explicit Board(Game *game);
    ~Board();

    void place(int i, SquareIcon icon, bool animated);
    void placeX(int i);
    void placeO(int i = AI_MOVE);
    bool isFrozen() const;

private:
    void setFrozen(bool frozen);
    void endRound();
    void graySquares();
    void displayWinner();

    static constexpr int AI_MOVE{-1};
    static constexpr int NEXT_ROUND_DELAY{4000};

    Game *game{nullptr};
    GameBar *gameBar{nullptr};
    Square **squares{nullptr};
    GameAI *ai{nullptr};
    bool frozen{false};
};
