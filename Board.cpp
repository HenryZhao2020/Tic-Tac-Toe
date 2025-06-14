#include "Board.h"
#include "Attr.h"
#include "GameUtil.h"
#include "Game.h"
#include "GameAI.h"
#include "GameBar.h"
#include "ScoreBoard.h"
#include "Square.h"

#include <QGridLayout>
#include <QTimer>

Board::Board(Game *game)
    : QFrame{game}, game{game}, gameBar{game->getGameBar()},
      squares{new Square *[BOARD_DIM * BOARD_DIM]{nullptr}},
      ai{new GameAI} {
    auto boardLayout = new QGridLayout{this};
    boardLayout->setSpacing(0);
    boardLayout->setContentsMargins(0, 0, 0, 0);

    for (int i = 0; i < BOARD_DIM * BOARD_DIM; ++i) {
        squares[i] = new Square{this, i};

        const int row{i / BOARD_DIM};
        const int col{i % BOARD_DIM};
        boardLayout->addWidget(squares[i], row * 2, col * 2);

        if (row != BOARD_DIM - 1) {
            static const int colSpan{BOARD_DIM * 2 - 1};
            auto hLine = new QFrame{this};
            hLine->setObjectName("line");
            boardLayout->addWidget(hLine, row * 2 + 1, 0, 1, colSpan);
        }

        if (col != BOARD_DIM - 1) {
            auto vLine = new QFrame{this};
            vLine->setObjectName("line");
            boardLayout->addWidget(vLine, row * 2, col * 2 + 1);
        }
    }
}

Board::~Board() {
    delete ai;
    delete[] squares;
}

void Board::place(int i, SquareIcon icon, bool animated) {
    setFrozen(true);
    disconnect(squares[i], &Square::clicked, nullptr, nullptr);
    squares[i]->placeIcon(icon, animated);

    QTimer::singleShot(TURN_DELAY, this, [this] { setFrozen(false); });
}

void Board::placeX(int i) {
    place(i, SquareIcon::X, Attr::getSettings().animated);
    Attr::getProgress().board[i] = SquareIcon::X;
    Attr::getProgress().xTurn = false;

    if (ai->isRoundEnded()) {
        endRound();
        return;
    }

    gameBar->setInfoVisible(false);
    QTimer::singleShot(TURN_DELAY, this, [this] { gameBar->setInfoText(tr("O's turn")); });

    if (!Attr::getSettings().twoPlayer) {
        QTimer::singleShot(TURN_DELAY * 2, this, [this] { placeO(); });
    }
}

void Board::placeO(int i) {
    if (i == AI_MOVE) i = ai->getNextMoveAI();

    place(i, SquareIcon::O, Attr::getSettings().animated);
    Attr::getProgress().board[i] = SquareIcon::O;
    Attr::getProgress().xTurn = true;

    if (ai->isRoundEnded()) {
        endRound();
        return;
    }

    gameBar->setInfoVisible(!Attr::getSettings().twoPlayer);
    QTimer::singleShot(TURN_DELAY, this, [this] { gameBar->setInfoText(tr("X's turn")); });
}

void Board::setFrozen(bool frozen) {
    this->frozen = frozen;
}

bool Board::isFrozen() const {
    return frozen;
}

void Board::endRound() {
    setFrozen(false);
    Attr::getProgress().ended = true;

    graySquares();
    displayWinner();
    game->getScoreBoard()->updateValues();
}

void Board::graySquares() {
    for (int i = 0; i < BOARD_DIM * BOARD_DIM; ++i) {
        if (!ai->getWinSeq() || !ai->getWinSeq()->contains(i)) {
            squares[i]->placeIcon(Attr::getProgress().board[i], false, true);
        }
        disconnect(squares[i], &Square::clicked, nullptr, nullptr);
        connect(squares[i], &Square::clicked, game, &Game::restart);
    }

    if (ai->getWinner() != SquareIcon::NONE && Attr::getSettings().animated) {
        for (const int &i : *ai->getWinSeq()) {
            squares[i]->flash();
        }
    }
}

void Board::displayWinner() {
    QIcon icon;
    QString text;

    if (ai->getWinner() == SquareIcon::X) {
        icon = IconUtil::load(":/icons/Win.svg");
        text = tr("The winner is X!");
        ++Attr::getStats().xPoint;
    } else if (ai->getWinner() == SquareIcon::O) {
        bool twoPlayer{Attr::getSettings().twoPlayer};
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
    QTimer::singleShot(NEXT_ROUND_DELAY, this, [this] {
        gameBar->setInfoText(tr("Click any square to restart..."));
    });
}
