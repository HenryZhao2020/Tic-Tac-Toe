#pragma once

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class Game;

/**
 * @brief Displays the game status and control buttons.
 *
 * GameBar shows a game status message on the left (e.g., current turn or result)
 * and provides control buttons such as Restart, Settings, Statistics, and Help
 * for in-game configuration and assistance.
 */
class GameBar : public QFrame {
    Q_OBJECT

public:
    explicit GameBar(Game *game);

    void setInfoIcon(const QIcon &icon);
    void setInfoText(const QString &text);
    void setInfoVisible(bool visible);
    void setRestartEnabled(bool enabled);

private:
    template<typename Callable>
    QPushButton *newMenuButton(const QIcon &icon, const QString &tip, const Callable &call);

    QHBoxLayout *barLayout;
    QPushButton *iconButton;
    QLabel *infoLabel;
    QPushButton *restartButton;
};
