#include "Square.h"
#include "Animation.h"
#include "Attr.h"
#include "GameUtil.h"
#include "Board.h"

const QIcon &Square::getIcon(SquareIcon icon, bool gray) {
    static const QIcon X = IconUtil::load(":/icons/X.svg");
    static const QIcon X_GRAY = IconUtil::load(":/icons/X_Gray.svg");
    static const QIcon O = IconUtil::load(":/icons/O.svg");
    static const QIcon O_GRAY = IconUtil::load(":/icons/O_Gray.svg");
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

Square::Square(Board *board, int i) : QPushButton(board), animator{new IconAnimation{this}} {
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
    if (animator) delete animator;
}

void Square::placeIcon(SquareIcon icon, bool animated, bool gray) {
    setIcon(getIcon(icon, gray));

    const int iconSize = width() * ICON_SCALE;
    if (animated) {
        animator->zoomIn(iconSize);
    } else {
        setIconSize(QSize(iconSize, iconSize));
    }
}

void Square::flash() {
    animator->flash();
}
