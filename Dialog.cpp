#include "Dialog.h"
#include "Attr.h"
#include "Board.h"
#include "GameUtil.h"
#include "Game.h"
#include "GameBar.h"
#include "ScoreBoard.h"
#include "AppInfo.h"

#include <QGroupBox>
#include <QTextBrowser>
#include <QFile>
#include <QProcess>
#include <QShortcut>
#include <QDesktopServices>

Dialog::Dialog(Game *game, const QIcon &icon, const QString &title)
    : QDialog{game}, game{game}, mainFrame{new QFrame{this}},
      okButton{new QPushButton{tr("OK"), this}} {
    setWindowIcon(icon);
    setWindowTitle(title);
    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);

    auto mainLayout = new QVBoxLayout{this};
    mainLayout->setSpacing(30);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->addWidget(mainFrame);

    auto buttonFrame = new QFrame{this};
    mainLayout->addSpacing(30);
    mainLayout->addWidget(buttonFrame, 0, Qt::AlignCenter);

    connect(okButton, &QPushButton::clicked, this, &Dialog::close);

    buttonLayout = new QHBoxLayout{buttonFrame};
    buttonLayout->setSpacing(10);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->addWidget(okButton);

    // Use <Cmd+W> to close window in macOS
    auto *closeShortcut = new QShortcut{QKeySequence::Close, this};
    connect(closeShortcut, &QShortcut::activated, this, &Dialog::close);
}

Dialog::~Dialog() {}

void Dialog::show() {
    setFixedSize(sizeHint());
    QDialog::show();
}

void Dialog::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        deleteLater();
    }
    QDialog::keyPressEvent(event);
}

SettingsDialog::SettingsDialog(Game *game)
    : Dialog{game, IconUtil::load(":/icons/Settings.svg"), tr("Settings")},
      settingsLayout{new QVBoxLayout{mainFrame}},
      langBox{new QComboBox{this}}, resetBox{new QComboBox{this}} {
    settingsLayout->setSpacing(15);

    auto gameGroup = newGroup(tr("Game"));
    addCheckBox(gameGroup, tr("Two player mode"), Attr::getSettings().twoPlayer);
    addCheckBox(gameGroup, tr("Enable game animation"), Attr::getSettings().animated);
    addCheckBox(gameGroup, tr("Display scores on bottom"), Attr::getSettings().showScores);

    auto langGroup = newGroup(tr("Language"));
    for (const Lang &lang : LangUtil::getLanguages()) {
        langBox->addItem(LangUtil::getLangName(lang));
    }
    langBox->setCurrentIndex(static_cast<int>(Attr::getSettings().lang));
    langGroup->addWidget(langBox);

    resetBox->addItems({DO_NOT_RESET, RESET_EVERYTHING, RESET_SETTINGS, RESET_POINTS});
    buttonLayout->insertWidget(0, resetBox);

    connect(okButton, &QPushButton::clicked, this, &SettingsDialog::applySettings);

    auto cancelButton = new QPushButton{tr("Cancel"), this};
    connect(cancelButton, &QPushButton::clicked, this, &Dialog::close);
    buttonLayout->addWidget(cancelButton);
}

SettingsDialog::~SettingsDialog() {}

QVBoxLayout *SettingsDialog::newGroup(const QString &title) {
    auto group = new QGroupBox{title, this};
    settingsLayout->addWidget(group);

    auto groupLayout = new QVBoxLayout{group};
    groupLayout->setSpacing(10);
    groupLayout->setContentsMargins(20, 20, 20, 20);
    return groupLayout;
}

void SettingsDialog::addCheckBox(QLayout *layout, const QString &text, bool &state) {
    auto box = new QCheckBox{text, this};
    box->setChecked(state);
    checkBoxes.insert(box, &state);
    layout->addWidget(box);
}

void SettingsDialog::applySettings() {
    for (auto it = checkBoxes.begin(); it != checkBoxes.end(); ++it) {
        *it.value() = it.key()->isChecked();
    }

    const QString resetOption{resetBox->currentText()};
    if (resetOption == RESET_EVERYTHING) {
        qApp->quit();
        QFile::remove("TTT_Data");
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    } else if (resetOption == RESET_SETTINGS) {
        Attr::resetSettings();
    } else if (resetOption == RESET_POINTS) {
        Attr::resetStats();
        game->getScoreBoard()->updateValues();
    }

    if (!Attr::getProgress().ended) {
        game->resumeRound();
    }

    // Show/Hide score board
    game->getScoreBoard()->updateHeaders();
    game->getScoreBoard()->setVisible(Attr::getSettings().showScores);
    game->centralWidget()->adjustSize();
    game->setFixedSize(game->sizeHint());

    // Apply language
    const Lang oldLang{Attr::getSettings().lang};
    const Lang newLang{langBox->currentIndex()};
    Attr::getSettings().lang = newLang;
    if (newLang != oldLang) {
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }
}

AboutDialog::AboutDialog(Game *game)
    : Dialog{game, IconUtil::load(":/icons/Help.svg"), tr("About") + " " + AppInfo::name()} {
    auto sideLayout = new QHBoxLayout{mainFrame};
    sideLayout->setSpacing(40);
    sideLayout->addWidget(createInfoFrame());

    auto helpPage = new QTextBrowser{this};
    helpPage->setHtml(AppInfo::description());
    sideLayout->addWidget(helpPage);
}

AboutDialog::~AboutDialog() {}

QFrame *AboutDialog::createInfoFrame() {
    auto infoFrame = new QFrame{this};
    auto infoLayout = new QVBoxLayout{infoFrame};
    infoLayout->setSpacing(8);

    auto logoButton = new QPushButton{this};
    logoButton->setIcon(AppInfo::icon());
    logoButton->setIconSize(QSize{180, 180});
    logoButton->setObjectName("borderless");
    infoLayout->addWidget(logoButton, 0, Qt::AlignCenter);

    auto titleLabel = new QLabel{AppInfo::name(), this};
    titleLabel->setObjectName("title");
    infoLayout->addWidget(titleLabel, 0, Qt::AlignCenter);

    auto versionLabel = new QLabel{tr("Version") + ": " + AppInfo::version(), this};
    infoLayout->addWidget(versionLabel, 0, Qt::AlignCenter);

    auto devLabel = new QLabel{tr("Developer") + ": " + AppInfo::developer(), this};
    infoLayout->addWidget(devLabel, 0, Qt::AlignCenter);

    auto linkButton = new QPushButton{tr("Visit my GitHub"), this};
    linkButton->setObjectName("link");
    linkButton->setCursor(Qt::PointingHandCursor);
    connect(linkButton, &QPushButton::clicked, []() {
        QDesktopServices::openUrl(AppInfo::github());
    });
    infoLayout->addWidget(linkButton, 0, Qt::AlignCenter);
    infoLayout->addStretch();

    return infoFrame;
}
