QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Animation.cpp \
    AppInfo.cpp \
    Attr.cpp \
    Board.cpp \
    Dialog.cpp \
    Game.cpp \
    GameAI.cpp \
    GameBar.cpp \
    GameUtil.cpp \
    Lang.cpp \
    Main.cpp \
    ScoreBoard.cpp \
    Square.cpp

HEADERS += \
    Animation.h \
    AppInfo.h \
    Attr.h \
    Board.h \
    Dialog.h \
    Game.h \
    GameAI.h \
    GameBar.h \
    GameUtil.h \
    Lang.h \
    ScoreBoard.h \
    Square.h

include(SingleApplication-3.5.2/singleapplication.pri)
DEFINES += QAPPLICATION_CLASS=QApplication

TARGET = "Tic Tac Toe"
VERSION = "3.1.0"

win32 {
    RC_ICONS = "TTT.ico"
    QMAKE_TARGET_DESCRIPTION = "Tic Tac Toe"
} mac {
    ICON = "TTT.icns"
}

TRANSLATIONS += \
    TTT_zh_CN.ts \
    TTT_zh_TW.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    TTT.qrc
