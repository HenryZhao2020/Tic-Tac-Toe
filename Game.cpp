#include "Game.h"
#include "Attr.h"
#include "GameUtil.h"
#include "Board.h"
#include "GameBar.h"
#include "ScoreBoard.h"
#include "Square.h"
#include "Dialog.h"
#include "AppInfo.h"

#include <QTimer>
#include <QShortcut>
#include <QMenuBar>

Game::Game() : gameBar{new GameBar{this}}, board{new Board{this}},
               scoreBoard{new ScoreBoard{this}} {
    setWindowTitle(AppInfo::name());

    auto mainWidget = new QWidget{this};
    setCentralWidget(mainWidget);

    mainLayout = new QVBoxLayout{mainWidget};
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(35, 35, 35, 35);

    gameBar->setInfoIcon(IconUtil::load(":/icons/Bell.svg"));
    gameBar->setInfoText(tr("Click a square to begin..."));
    gameBar->setRestartEnabled(false);
    mainLayout->addWidget(gameBar);

    mainLayout->addWidget(board, 1, Qt::AlignCenter);

    scoreBoard->updateHeaders();
    scoreBoard->updateValues();
    mainLayout->addWidget(scoreBoard, 0, Qt::AlignCenter);

    // Use <Cmd+W> to close window in macOS
    auto *closeShortcut = new QShortcut{QKeySequence::Close, this};
    connect(closeShortcut, &QShortcut::activated, this, &QWidget::close);

#ifdef Q_OS_MAC
    // Create "About" action
    auto aboutAction = new QAction{"About " + AppInfo::name(), this};
    aboutAction->setMenuRole(QAction::AboutRole);
    connect(aboutAction, &QAction::triggered, this, [this] {
        auto dialog = new AboutDialog{this};
        dialog->show();
    });
    // Add dummy menus to force proper layout
    auto dummyMenu = menuBar()->addMenu("File");
    // Will appear in macOS app menu instead of File
    dummyMenu->addAction(aboutAction);
#endif
}

void Game::show() {
    setFixedSize(sizeHint());
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(),
                                    screen()->availableGeometry()));
    QMainWindow::show();
}

GameBar *Game::getGameBar() const {
    return gameBar;
}

Board *Game::getBoard() const {
    return board;
}

ScoreBoard *Game::getScoreBoard() const {
    return scoreBoard;
}

void Game::restart() {
    Attr::resetProgress();

    mainLayout->removeWidget(board);
    board->deleteLater();

    board = new Board{this};
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

    for (int i = 0; i < BOARD_DIM * BOARD_DIM; ++i) {
        const SquareIcon icon{Attr::getProgress().board[i]};
        if (icon != SquareIcon::NONE) {
            board->place(i, icon, false);
        }
    }

    scoreBoard->updateHeaders();
    scoreBoard->updateValues();
    scoreBoard->setVisible(Attr::getSettings().showScores);

    resumeRound();
    setFixedSize(sizeHint());
}

void Game::resumeRound() {
    const bool xTurn{Attr::getProgress().xTurn};
    gameBar->setInfoText(xTurn ? tr("X's turn") : tr("O's turn"));

    if (!xTurn && !Attr::getSettings().twoPlayer) {
        QTimer::singleShot(Board::TURN_DELAY, this, [this] { board->placeO(); });
    }
}
