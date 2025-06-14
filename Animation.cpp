#include "Animation.h"

TextAnimation::~TextAnimation() {
    if (typewriteTimer) typewriteTimer->deleteLater();
}

void TextAnimation::typewrite(const QString &text, int interval) {
    if (typewriteTimer) typewriteTimer->deleteLater();

    int length{0};
    setTextFunc("");

    typewriteTimer = new QTimer{widget};
    QTimer::connect(typewriteTimer, &QTimer::timeout, widget, [=, this]() mutable {
        if (length >= text.length()) {
            typewriteTimer->deleteLater();
            typewriteTimer = nullptr;
            return;
        }
        setTextFunc(text.left(++length));
    });
    typewriteTimer->start(interval);
}

IconAnimation::~IconAnimation() {
    if (zoomInTimer) zoomInTimer->deleteLater();
    if (flashTimer) flashTimer->deleteLater();
}

void IconAnimation::zoomIn(int finalSize, int interval) {
    if (zoomInTimer) zoomInTimer->deleteLater();

    int currSize{finalSize % interval};
    const int increment{finalSize / interval};
    setIconSizeFunc(QSize{currSize, currSize});

    zoomInTimer = new QTimer{widget};
    QTimer::connect(zoomInTimer, &QTimer::timeout, widget, [=, this]() mutable {
        if (currSize >= finalSize) {
            zoomInTimer->deleteLater();
            zoomInTimer = nullptr;
            return;
        }
        currSize += increment;
        setIconSizeFunc(QSize{currSize, currSize});
    });
    zoomInTimer->start(interval);
}

void IconAnimation::flash(int flashes, int interval) {
    if (flashTimer) flashTimer->deleteLater();

    flashes *= 2;
    const QIcon origIcon{iconFunc()};
    static const QIcon blankIcon;

    flashTimer = new QTimer{widget};
    QTimer::connect(flashTimer, &QTimer::timeout, widget, [=, this]() mutable {
        if (flashes <= 0) {
            flashTimer->deleteLater();
            flashTimer = nullptr;
            return;
        }
        setIconFunc((--flashes % 2) ? blankIcon : origIcon);
    });
    flashTimer->start(interval);
}
