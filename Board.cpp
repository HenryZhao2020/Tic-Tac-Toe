#include "Board.h"
#include "Attr.h"
#include "GameUtil.h"
#include "Game.h"
#include "GameAI.h"
#include "GameBar.h"
#include "ScoreBoard.h"
#include "Square.h"

#include <QGridLayout>

GameAI Board::ai {Attr::getProgress().board};

Board::Board(Game *game)
    : QFrame(game), game{game}, gameBar{game->getGameBar()} {
    setFrozen(false);

    auto boardLayout = new QGridLayout(this);
    boardLayout->setSpacing(0);
    boardLayout->setContentsMargins(0, 0, 0, 0);

    for (int i = 0; i < 9; ++i) {
        squares[i] = new Square(this, i);

        int row = i / 3;
        int col = i % 3;
        boardLayout->addWidget(squares[i], row * 2, col * 2);

        if (row != 2) {
            auto hLine = new QFrame(this);
            hLine->setObjectName("line");
            boardLayout->addWidget(hLine, row * 2 + 1, 0, 1, 5);
        }

        if (col != 2) {
            auto vLine = new QFrame(this);
            vLine->setObjectName("line");
            boardLayout->addWidget(vLine, row * 2, col * 2 + 1);
        }
    }
}

Board::~Board() {}

void Board::place(int i, const QIcon &icon, bool animated) {
    setFrozen(true);

    if (i < 0 || i > 8) {
        qCritical() << "Square index out of bounds!";
    }

    disconnect(squares[i], &Square::clicked, nullptr, nullptr);
    squares[i]->setIcon(icon);

    int size = squares[i]->width() * 0.70;
    if (animated) {
        squares[i]->zoomIn(size);
    } else {
        squares[i]->setIconSize(QSize(size, size));
    }

    QTimer::singleShot(500, this, [this] { setFrozen(false); });
}

void Board::placeX(int i) {
    place(i, Square::getIcon(SquareIcon::X), Attr::getSettings().animated);
    Attr::getProgress().board[i] = SquareIcon::X;
    Attr::getProgress().xTurn = false;

    if (isRoundEnded()) {
        endRound();
        return;
    }

    gameBar->setInfoVisible(false);
    QTimer::singleShot(500, this, [this] { gameBar->setInfoText(tr("O's turn")); });

    if (!Attr::getSettings().twoPlayer) {
        QTimer::singleShot(1000, this, [this] { placeO(); });
    }
}

void Board::placeO(int i) {
    if (i == -1) {
        i = ai.getBestMove();
    }

    place(i, Square::getIcon(SquareIcon::O), Attr::getSettings().animated);
    Attr::getProgress().board[i] = SquareIcon::O;
    Attr::getProgress().xTurn = true;

    if (isRoundEnded()) {
        endRound();
        return;
    }

    gameBar->setInfoVisible(!Attr::getSettings().twoPlayer);
    QTimer::singleShot(500, this, [this] { gameBar->setInfoText(tr("X's turn")); });
}

void Board::setFrozen(bool frozen) {
    this->frozen = frozen;
}

bool Board::isFrozen() {
    return frozen;
}

bool Board::isRoundEnded() {
    winner = ai.getWinner();
    return winner != SquareIcon::EMPTY || ai.isDraw();
}

void Board::endRound() {
    setFrozen(false);
    Attr::getProgress().ended = true;

    const QList<int> &winSeq = ai.getWinSeq();
    for (int i = 0; i < 9; ++i) {
        SquareIcon icon = Attr::getProgress().board[i];
        if (icon == SquareIcon::X && !winSeq.contains(i)) {
            squares[i]->setIcon(Square::getIcon(SquareIcon::X, true));
        } else if (icon == SquareIcon::O && !winSeq.contains(i)) {
            squares[i]->setIcon(Square::getIcon(SquareIcon::O, true));
        } else {
            disconnect(squares[i], &Square::clicked, nullptr, nullptr);
        }
        connect(squares[i], &Square::clicked, game, &Game::restart);
    }

    for (int i : winSeq) {
        squares[i]->flash();
    }

    QIcon icon;
    QString text;

    if (winner == SquareIcon::X) {
        icon = IconUtil::load(":/icons/Win.svg");
        text = tr("The winner is X!");
        ++Attr::getStats().xPoint;
    } else if (winner == SquareIcon::O) {
        bool twoPlayer = Attr::getSettings().twoPlayer;
        icon = IconUtil::load(twoPlayer ? ":/icons/Win.svg" : ":/icons/AI.svg");
        text = tr("The winner is O!");
        ++Attr::getStats().oPoint;
    } else {
        icon = IconUtil::load(":/icons/Tie.svg");
        text = tr("Tie!");
        ++Attr::getStats().tiePoint;
    }

    gameBar->setInfoIcon(icon);
    gameBar->setInfoText(text);
    gameBar->setRestartEnabled(true);
    QTimer::singleShot(4000, this, [this] {
        gameBar->setInfoText(tr("Click any square to restart..."));
    });

    game->getScoreBoard()->updateValues();
}
