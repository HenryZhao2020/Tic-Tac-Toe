#include "Attr.h"

#include <QMap>
#include <QLocale>
#include <QDataStream>
#include <QFile>

Attr::Progress Attr::progress;
Attr::Settings Attr::settings;
Attr::Stats Attr::stats;

const QStringList &Lang::getLangNames() {
    static const QStringList names {
        QLocale::languageToString(QLocale::English),
        QLocale::languageToString(QLocale::Chinese) + " (简体)",
        QLocale::languageToString(QLocale::Chinese) + " (繁體)"
    };
    return names;
}

QString Lang::getLangTerrCode(const Name &name) {
    static const QMap<Name, QLocale> locales {
        { Lang::ENGLISH,      QLocale(QLocale::English, QLocale::UnitedStates) },
        { Lang::CHINESE_SIMP, QLocale(QLocale::Chinese, QLocale::China) },
        { Lang::CHINESE_TRAD, QLocale(QLocale::Chinese, QLocale::Taiwan) },
    };
    return locales.value(name).name();
}

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
    QFile file("TTT_Data");
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return;
    }

    QDataStream out(&file);
    out << progress.board << progress.xTurn << progress.ended;
    out << settings.lang << settings.animated << settings.twoPlayer << settings.showScores;
    out << stats.xPoint << stats.tiePoint << stats.oPoint;
    file.close();
}

bool Attr::loadAttr() {
    QFile file("TTT_Data");
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }

    QDataStream in(&file);
    in >> progress.board >> progress.xTurn >> progress.ended;
    in >> settings.lang >> settings.animated >> settings.twoPlayer >> settings.showScores;
    in >> stats.xPoint >> stats.tiePoint >> stats.oPoint;
    file.close();
    return true;
}
