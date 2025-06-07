#include "GameUtil.h"

#include <QFile>
#include <QHash>

QHash<QString, QIcon> IconUtil::iconCache;
QHash<QString, QIcon> IconUtil::grayCache;

QString FileUtil::readAll(const QString &path) {
    QFile file(path);
    QString content;
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        content = file.readAll();
        file.close();
    }
    return content;
}

const QIcon &IconUtil::load(const QString &path) {
    auto it = iconCache.find(path);
    if (it != iconCache.end()) {
        return it.value();
    }
    return iconCache.insert(path, QIcon(path)).value();
}

const QIcon &IconUtil::gray(const QString &path) {
    auto it = grayCache.find(path);
    if (it != grayCache.end()) {
        return it.value();
    }

    QImage image(path);
    if (image.isNull()) {
        static const QIcon emptyIcon;
        return emptyIcon;
    }

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color = image.pixelColor(x, y);
            int gray = qGray(color.rgb());
            color.setRgb(gray, gray, gray, color.alpha());
            image.setPixelColor(x, y, color);
        }
    }

    return grayCache.insert(path, QPixmap::fromImage(image)).value();
}

