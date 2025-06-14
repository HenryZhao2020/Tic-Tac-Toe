#include "Attr.h"

#include <QFile>
#include <QDataStream>

Attr::Progress Attr::progress;
Attr::Settings Attr::settings;
Attr::Stats Attr::stats;

Attr::Progress &Attr::getProgress() {
    return progress;
}

Attr::Settings &Attr::getSettings() {
    return settings;
}

Attr::Stats &Attr::getStats() {
    return stats;
}

void Attr::resetProgress() {
    progress = {};
}

void Attr::resetSettings() {
    settings = {};
}

void Attr::resetStats() {
    stats = {};
}

void Attr::saveAttr() {
    QFile file{"TTT_Data"};
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return;
    }

    QDataStream out{&file};
    out << progress.board << progress.xTurn << progress.ended;
    out << settings.twoPlayer << settings.animated << settings.showScores << settings.lang;
    out << stats.xPoint << stats.tiePoint << stats.oPoint;
    file.close();
}

bool Attr::loadAttr() {
    QFile file{"TTT_Data"};
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }

    QDataStream in{&file};
    in >> progress.board >> progress.xTurn >> progress.ended;
    in >> settings.twoPlayer >> settings.animated >> settings.showScores >> settings.lang;
    in >> stats.xPoint >> stats.tiePoint >> stats.oPoint;
    file.close();
    return true;
}
