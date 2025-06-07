#pragma once

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>

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
    ~GameBar();

    void setInfoIcon(const QIcon &icon);
    void setInfoText(const QString &text);
    void setInfoVisible(bool visible);
    void setRestartEnabled(bool enabled);

private:
    template<typename Callable>
    QPushButton *newMenuButton(const QIcon &icon, const QString &tip, const Callable &call);
    void typewriteInfo(const QString &text);

    QHBoxLayout *barLayout;
    QPushButton *iconButton;
    QLabel *infoLabel;
    QPushButton *restartButton;

    QTimer *infoTimer = nullptr;
    int infoLength;
};
