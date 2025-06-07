#include "Game.h"
#include "Attr.h"
#include "GameUtil.h"
#include "Board.h"
#include "GameBar.h"
#include "ScoreBoard.h"
#include "Square.h"

#include <QTimer>

Game::Game() {
    setWindowTitle(tr("Tic Tac Toe"));

    auto mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(35, 35, 35, 35);

    gameBar = new GameBar(this);
    gameBar->setInfoIcon(IconUtil::load(":/icons/Bell.svg"));
    gameBar->setInfoText(tr("Click a square to begin..."));
    gameBar->setRestartEnabled(false);
    mainLayout->addWidget(gameBar);

    board = new Board(this);
    mainLayout->addWidget(board, 1, Qt::AlignCenter);

    scoreBoard = new ScoreBoard(this);
    scoreBoard->updateHeaders();
    scoreBoard->updateValues();
    mainLayout->addWidget(scoreBoard, 0, Qt::AlignCenter);

    setFixedSize(sizeHint());
}

Game::~Game() {}

GameBar *Game::getGameBar() {
    return gameBar;
}

Board *Game::getBoard() {
    return board;
}

ScoreBoard *Game::getScoreBoard() {
    return scoreBoard;
}

void Game::restart() {
    Attr::resetProgress();

    mainLayout->removeWidget(board);
    board->deleteLater();

    board = new Board(this);
    mainLayout->insertWidget(1, board, 1, Qt::AlignCenter);

    gameBar->setInfoIcon(IconUtil::load(":/icons/Bell.svg"));
    gameBar->setRestartEnabled(false);

    resumeRound();
}

void Game::loadSave() {
    if (Attr::getProgress().ended) {
        restart();
        return;
    }

    for (int i = 0; i < 9; ++i) {
        SquareIcon icon = Attr::getProgress().board[i];
        if (icon != SquareIcon::EMPTY) {
            board->place(i, Square::getIcon(icon), false);
        }
    }

    scoreBoard->updateHeaders();
    scoreBoard->updateValues();
    scoreBoard->setVisible(Attr::getSettings().showScores);

    resumeRound();
    setFixedSize(sizeHint());
}

void Game::resumeRound() {
    bool xTurn = Attr::getProgress().xTurn;
    gameBar->setInfoText(xTurn ? tr("X's turn") : tr("O's turn"));

    if (!xTurn && !Attr::getSettings().twoPlayer) {
        QTimer::singleShot(500, this, [this] { board->placeO(); });
    }
}
