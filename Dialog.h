#pragma once

#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QHash>
#include <QKeyEvent>
#include <QPushButton>
#include <QTextBrowser>

class Game;

/**
 * @brief Base dialog class for game customization and information display.
 *
 * Provides a reusable foundation for dialogs used in the game, such as
 * settings configuration or about/info screens.
 */
class Dialog : public QDialog {
    Q_OBJECT

public:
    explicit Dialog(Game *game, const QIcon &icon, const QString &title);
    ~Dialog();

protected:
    void keyPressEvent(QKeyEvent *event) override;

    Game *game;
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    QPushButton *okButton;
};

/**
 * @brief Dialog for configuring game settings.
 *
 * Inherits from Dialog and provides user interface elements for customizing
 * gameplay options such as language, animations, and game mode.
 */
class SettingsDialog : public Dialog {
    Q_OBJECT

public:
    explicit SettingsDialog(Game *game);
    ~SettingsDialog();

private:
    QVBoxLayout *newGroup(const QString &title);
    void addCheckBox(QLayout *layout, const QString &text, bool &state);
    void applySettings();

    QHash<QCheckBox *, bool *> boxes;
    QComboBox *langBox;
    QComboBox *resetBox;
};

/**
 * @brief Dialog for displaying help and instructions.
 *
 * Inherits from Dialog and provides information about how to play the game,
 * rules, and general usage guidance for the player.
 */
class HelpDialog : public Dialog {
    Q_OBJECT

public:
    explicit HelpDialog(Game *game);
    ~HelpDialog();

private:
    QTextBrowser *newBrowser(const QString &path);
};
