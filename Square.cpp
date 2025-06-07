#include "Square.h"
#include "Attr.h"
#include "GameUtil.h"
#include "Board.h"

const QIcon &Square::getIcon(SquareIcon icon, bool gray) {
    static const QIcon &X = IconUtil::load(":/icons/X.svg");
    static const QIcon &X_GRAY = IconUtil::gray(":/icons/X.svg");
    static const QIcon &O = IconUtil::load(":/icons/O.svg");
    static const QIcon &O_GRAY = IconUtil::gray(":/icons/O.svg");
    static const QIcon EMPTY;

    switch (icon) {
    case SquareIcon::X:
        return gray ? X_GRAY : X;
    case SquareIcon::O:
        return gray ? O_GRAY : O;
    default:
        return EMPTY;
    }
}

Square::Square(Board *board, int i) : QPushButton(board) {
    setCursor(Qt::PointingHandCursor);
    connect(this, &Square::clicked, this, [board, i] {
        if (board->isFrozen()) {
            return;
        }

        if (Attr::getProgress().xTurn) {
            board->placeX(i);
        } else if (Attr::getSettings().twoPlayer) {
            board->placeO(i);
        }
    });
}

Square::~Square() {
    if (zoomTimer != nullptr) {
        zoomTimer->deleteLater();
    }

    if (flashTimer != nullptr) {
        flashTimer->deleteLater();
    }
}

void Square::zoomIn(int maxSize) {
    if (zoomTimer != nullptr) {
        zoomTimer->deleteLater();
    }
    currSize = maxSize % 10;
    setIconSize(QSize(currSize, currSize));

    zoomTimer = new QTimer(this);
    connect(zoomTimer, &QTimer::timeout, this, [this, maxSize] {
        if (currSize >= maxSize) {
            zoomTimer->deleteLater();
            zoomTimer = nullptr;
            return;
        }

        currSize += maxSize / 10;
        setIconSize(QSize(currSize, currSize));
    });
    zoomTimer->start(10);
}

void Square::flash(int maxFlash) {
    if (flashTimer != nullptr) {
        flashTimer->deleteLater();
    }
    currFlash = 0;

    const QIcon &oldIcon = icon();
    static QIcon blankIcon;

    flashTimer = new QTimer(this);
    connect(flashTimer, &QTimer::timeout, this, [this, maxFlash, oldIcon] {
        if (currFlash >= maxFlash) {
            flashTimer->deleteLater();
            flashTimer = nullptr;
            return;
        }

        setIcon(++currFlash % 2 ? blankIcon : oldIcon);
    });
    flashTimer->start(250);
}
