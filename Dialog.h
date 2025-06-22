#pragma once

#include <QDialog>
#include <QKeyEvent>
#include <QFrame>
#include <QBoxLayout>
#include <QPushButton>
#include <QMap>
#include <QCheckBox>
#include <QComboBox>

class Game;

/**
 * @brief Base dialog class for game customization and information display.
 */
class Dialog : public QDialog {
    Q_OBJECT

public:
    explicit Dialog(Game *game, const QIcon &icon, const QString &title);

    void show();

protected:
    void keyPressEvent(QKeyEvent *event) override;

    Game *game{nullptr};
    QFrame *mainFrame{nullptr};
    QHBoxLayout *buttonLayout{nullptr};
    QPushButton *okButton{nullptr};
};

/**
 * @brief Dialog for configuring game settings.
 */
class SettingsDialog : public Dialog {
    Q_OBJECT

public:
    explicit SettingsDialog(Game *game);

private:
    const QString DO_NOT_RESET{tr("Do not reset")};
    const QString RESET_EVERYTHING{tr("Reset everything")};
    const QString RESET_SETTINGS{tr("Reset settings")};
    const QString RESET_POINTS{tr("Reset points")};

    QVBoxLayout *newGroup(const QString &title);
    void addCheckBox(QLayout *layout, const QString &text, bool &state);
    void applySettings();

    QVBoxLayout *settingsLayout{nullptr};
    QMap<QCheckBox *, bool *> checkBoxes;
    QComboBox *langBox{nullptr};
    QComboBox *resetBox{nullptr};
};

/**
 * @brief Dialog for displaying help and game instructions.
 */
class AboutDialog : public Dialog {
    Q_OBJECT

public:
    explicit AboutDialog(Game *game);

private:
    QFrame *createInfoFrame();
};
