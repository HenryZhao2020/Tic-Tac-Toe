#pragma once

#include <QPushButton>
#include <QTimer>
#include <QIcon>

class Board;
enum class SquareIcon;

/**
 * @brief A clickable square that displays an 'X' or 'O' icon to indicate player turns.
 */
class Square : public QPushButton {
    Q_OBJECT

public:
    static const QIcon &getIcon(SquareIcon icon, bool gray = false);

    explicit Square(Board *board, int i);
    ~Square();

    void zoomIn(int maxSize);
    void flash(int maxFlash = 4);

private:
    QTimer *zoomTimer = nullptr;
    int currSize;

    QTimer *flashTimer = nullptr;
    int currFlash;
};
