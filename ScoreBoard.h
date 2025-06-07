#pragma once

#include <QFrame>
#include <QLabel>

class Game;

/**
 * @brief Displays, from left to right order, the current score of player X, number of ties,
 * and score of player O.
 */
class ScoreBoard : public QFrame {
    Q_OBJECT

public:
    explicit ScoreBoard(Game *game);
    ~ScoreBoard();

    void updateHeaders();
    void updateValues();

private:
    QLabel *xHead;
    QLabel *tieHead;
    QLabel *oHead;

    QLabel *xVal;
    QLabel *tieVal;
    QLabel *oVal;
};
