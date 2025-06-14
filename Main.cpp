#include "Attr.h"
#include "Dialog.h"
#include "GameUtil.h"
#include "Game.h"
#include "SingleApplication"

#include <QDir>
#include <QFontDatabase>
#include <QLibraryInfo>
#include <QTimer>
#include <QTranslator>

#ifdef Q_OS_WINDOWS
#include <Windows.h>
#endif

static void raiseWidget(QWidget *widget) {
#ifdef Q_OS_WINDOWS
    HWND hwnd = (HWND) widget->winId();

    // check if widget is minimized to Windows task bar
    if (::IsIconic(hwnd)) {
        ::ShowWindow(hwnd, SW_RESTORE);
    }

    ::SetForegroundWindow(hwnd);
#else
    widget->show();
    widget->raise();
    widget->activateWindow();
#endif
}

int main(int argc, char *argv[]) {
#ifdef Q_OS_WINDOWS
    SingleApplication app{argc, argv, true};
    if (app.isSecondary()) {
        AllowSetForegroundWindow(DWORD(app.primaryPid()));
        app.sendMessage("RAISE_WIDGET");
        return 0;
    }
#else
    SingleApplication app{argc, argv, true};
#endif

    QDir::setCurrent(app.applicationDirPath());

    app.setStyle("Fusion");
    app.setStyleSheet(FileUtil::readAll(":/conf/Styles.qss"));
    QObject::connect(&app, &QApplication::aboutToQuit, &Attr::saveAttr);

    QFontDatabase::addApplicationFont(":/fonts/MontserratAlternates-Medium.ttf");
    QFontDatabase::addApplicationFont(":/fonts/ZCOOLKuaiLe-Regular.ttf");

    const bool loaded{Attr::loadAttr()};

    QTranslator baseTrans, appTrans;
    if (Attr::getSettings().lang != Lang::ENGLISH) {
        const QString basePath{QLibraryInfo::path(QLibraryInfo::TranslationsPath)};
        const QString code{LangUtil::getLangCode(Attr::getSettings().lang)};

        if (baseTrans.load("qtbase_" + code, basePath)) {
            app.installTranslator(&baseTrans);
        }
        if (appTrans.load(":/i18n/TTT_" + code)) {
            app.installTranslator(&appTrans);
        }
    }

    Game game;
    game.show();

#ifdef Q_OS_WINDOWS
    QObject::connect(&app, &SingleApplication::receivedMessage, &game, [&game]() {
        raiseWidget(&game);
    });
#else
    QObject::connect(&app, &SingleApplication::instanceStarted, &game, [&game]() {
        raiseWidget(&game);
    });
#endif

    if (loaded) {
        game.loadSave();
    } else {
        auto dialog = new AboutDialog{&game};
        QTimer::singleShot(500, dialog, &Dialog::show);
    }

    return app.exec();
}
