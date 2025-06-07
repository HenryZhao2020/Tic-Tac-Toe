#include "GameBar.h"
#include "Attr.h"
#include "Dialog.h"
#include "Game.h"
#include "GameUtil.h"

GameBar::GameBar(Game *game) : QFrame(game) {
    barLayout = new QHBoxLayout(this);
    barLayout->setSpacing(5);
    barLayout->setContentsMargins(0, 0, 0, 0);

    iconButton = new QPushButton(this);
    barLayout->addWidget(iconButton);
    barLayout->addSpacing(2);

    infoLabel = new QLabel(this);
    barLayout->addWidget(infoLabel);
    barLayout->addStretch();

    restartButton = newMenuButton(IconUtil::load(":/icons/Restart.svg"), tr("Restart"), [game] {
        game->restart();
    });

    newMenuButton(IconUtil::load(":/icons/Settings.svg"), tr("Settings"), [game] {
        auto dialog = new SettingsDialog(game);
        dialog->show();
    });

    newMenuButton(IconUtil::load(":/icons/Help.svg"), tr("Help"), [game] {
        auto dialog = new HelpDialog(game);
        dialog->show();
    });
}

GameBar::~GameBar() {
    if (infoTimer != nullptr) {
        infoTimer->deleteLater();
    }
}

void GameBar::setInfoIcon(const QIcon &icon) {
    iconButton->setIcon(icon);
    setInfoVisible(true);
}

void GameBar::setInfoText(const QString &text) {
    if (infoLabel->text() == text) {
        return;
    }

    if (Attr::getSettings().animated) {
        typewriteInfo(text);
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

void GameBar::typewriteInfo(const QString &text) {
    if (infoTimer != nullptr) {
        infoTimer->deleteLater();
    }
    infoLength = 0;
    infoLabel->setText("");

    infoTimer = new QTimer(this);
    connect(infoTimer, &QTimer::timeout, this, [this, text] {
        if (infoLength >= text.size()) {
            infoTimer->deleteLater();
            infoTimer = nullptr;
            return;
        }

        infoLabel->setText(text.first(++infoLength));
    });
    infoTimer->start(20);
}

template<typename Callable>
QPushButton *GameBar::newMenuButton(
    const QIcon &icon, const QString &tip, const Callable &call) {
    auto button = new QPushButton(icon, "", this);
    button->setToolTip(tip);
    button->setCursor(Qt::PointingHandCursor);
    connect(button, &QPushButton::clicked, this, call);
    barLayout->addWidget(button);

    return button;
}
