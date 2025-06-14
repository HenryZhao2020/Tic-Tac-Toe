#include "GameBar.h"
#include "Attr.h"
#include "Dialog.h"
#include "Game.h"
#include "GameUtil.h"
#include "Animation.h"

GameBar::GameBar(Game *game)
    : QFrame{game}, barLayout{new QHBoxLayout{this}},
      iconButton{new QPushButton{this}}, infoLabel{new QLabel{this}} {
    barLayout->setSpacing(5);
    barLayout->setContentsMargins(0, 0, 0, 0);
    barLayout->addWidget(iconButton);
    barLayout->addSpacing(2);
    barLayout->addWidget(infoLabel);
    barLayout->addStretch();

    restartButton = newMenuButton(IconUtil::load(":/icons/Restart.svg"), tr("New Round"),
                                  [game] { game->restart(); });

    newMenuButton(IconUtil::load(":/icons/Settings.svg"), tr("Settings"), [game] {
        auto dialog = new SettingsDialog{game};
        dialog->show();
    });

    newMenuButton(IconUtil::load(":/icons/Help.svg"), tr("About"), [game] {
        auto dialog = new AboutDialog{game};
        dialog->show();
    });
}

GameBar::~GameBar() {}

void GameBar::setInfoIcon(const QIcon &icon) {
    iconButton->setIcon(icon);
    setInfoVisible(true);
}

void GameBar::setInfoText(const QString &text) {
    if (infoLabel->text() == text) {
        return;
    }

    if (Attr::getSettings().animated) {
        static TextAnimation animator(infoLabel);
        animator.typewrite(text);
    } else {
        infoLabel->setText(text);
    }
    setInfoVisible(true);
}

void GameBar::setInfoVisible(bool visible) {
    iconButton->setVisible(visible);
    infoLabel->setVisible(visible);
}

void GameBar::setRestartEnabled(bool enabled) {
    restartButton->setVisible(enabled);
}

template<typename Callable>
QPushButton *GameBar::newMenuButton(
    const QIcon &icon, const QString &tip, const Callable &call) {
    auto button = new QPushButton{icon, "", this};
    button->setToolTip(tip);
    button->setCursor(Qt::PointingHandCursor);
    connect(button, &QPushButton::clicked, this, call);
    barLayout->addWidget(button);

    return button;
}
