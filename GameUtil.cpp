#include "GameUtil.h"

#include <QFile>

QMap<QString, QIcon> IconUtil::cache;

QString FileUtil::readAll(const QString &path) {
    QFile file{path};
    QString content;
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        content = file.readAll();
        file.close();
    }
    return content;
}

const QIcon &IconUtil::load(const QString &path) {
    const auto it{cache.find(path)};
    if (it != cache.end()) {
        return it.value();
    }
    return cache.insert(path, QIcon(path)).value();
}

void IconUtil::clearCache() {
    cache.clear();
}
