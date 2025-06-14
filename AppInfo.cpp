#include "AppInfo.h"
#include "GameUtil.h"

#include <QObject>

QString AppInfo::name() {
    return QObject::tr("Tic Tac Toe");
}

const QIcon &AppInfo::icon() {
    return IconUtil::load(":/icons/TTT_Logo.png");
}

QString AppInfo::version() {
    return "3.1.0";
}

QString AppInfo::developer() {
    return "Henry Zhao";
}

QString AppInfo::description() {
    return FileUtil::readAll(QObject::tr(":/help/Rules.html"));
}

QString AppInfo::github() {
    return "https://github.com/HenryZhao2020/Tic-Tac-Toe";
}
