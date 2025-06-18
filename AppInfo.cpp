#include "AppInfo.h"
#include "GameUtil.h"

#include <QObject>

const QString &AppInfo::name() {
    static const QString name{QObject::tr("Tic Tac Toe")};
    return name;
}

const QIcon &AppInfo::icon() {
    static const QIcon icon{IconUtil::load(":/icons/TTT_Logo.png")};
    return icon;
}

const QString &AppInfo::version() {
    static const QString ver{"3.1.0"};
    return ver;
}

const QString &AppInfo::developer() {
    static const QString dev{"Henry Zhao"};
    return dev;
}

const QString &AppInfo::description() {
    static const QString desc{FileUtil::readAll(QObject::tr(":/help/Rules.html"))};
    return desc;
}

const QString &AppInfo::github() {
    static const QString git{"https://github.com/HenryZhao2020/Tic-Tac-Toe"};
    return git;
}
