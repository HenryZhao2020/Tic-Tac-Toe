#include "ScoreBoard.h"
#include "Attr.h"
#include "Game.h"

#include <QGridLayout>

ScoreBoard::ScoreBoard(Game *game) : QFrame{game} {
    auto boardLayout = new QGridLayout{this};
    boardLayout->setHorizontalSpacing(0);
    boardLayout->setVerticalSpacing(10);
    boardLayout->setContentsMargins(0, 0, 0, 0);

    xHead = new QLabel{this};
    xHead->setObjectName("red");
    boardLayout->addWidget(xHead, 0, 0);

    tieHead = new QLabel{tr("TIE"), this};
    boardLayout->addWidget(tieHead, 0, 1);

    oHead = new QLabel{this};
    oHead->setObjectName("blue");
    boardLayout->addWidget(oHead, 0, 2);

    xVal = new QLabel{this};
    xVal->setObjectName("red");
    boardLayout->addWidget(xVal, 1, 0);

    tieVal = new QLabel{this};
    boardLayout->addWidget(tieVal, 1, 1);

    oVal = new QLabel{this};
    oVal->setObjectName("blue");
    boardLayout->addWidget(oVal, 1, 2);
}

void ScoreBoard::updateHeaders() {
    bool twoPlayer{Attr::getSettings().twoPlayer};
    xHead->setText(twoPlayer ? tr("PLAYER 1") : tr("YOU"));
    oHead->setText(twoPlayer ? tr("PLAYER 2") : tr("AI"));
}

void ScoreBoard::updateValues() {
    xVal->setText(QString::number(Attr::getStats().xPoint));
    tieVal->setText(QString::number(Attr::getStats().tiePoint));
    oVal->setText(QString::number(Attr::getStats().oPoint));
}
