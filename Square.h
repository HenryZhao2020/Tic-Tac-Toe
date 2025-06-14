#pragma once

#include <QPushButton>
#include <QIcon>

class Board;
class IconAnimation;
enum class SquareIcon;

/**
 * @brief A clickable square that displays an 'X' or 'O' icon to indicate player turns.
 */
class Square : public QPushButton {
    Q_OBJECT

public:
    explicit Square(Board *board, int i);
    ~Square();

    void placeIcon(SquareIcon icon, bool animated, bool gray = false);
    void flash();

private:
    static const QIcon &getIcon(SquareIcon icon, bool gray = false);
    static constexpr double ICON_SCALE{0.70};

    IconAnimation *animator{nullptr};
};
